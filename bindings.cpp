#include <node_api.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>
#include <apt-pkg/pkgrecords.h>
#include <apt-pkg/init.h>
#include <apt-pkg/tagfile.h>
#include <apt-pkg/pkgsystem.h>
#include <apt-pkg/policy.h>

#include <vector>
#include <utility>
using namespace std;

namespace debstep {

  napi_value GetPackageTags(napi_env env, napi_callback_info info);
  std::vector<std::pair<std::string, std::string>> packageInfo(const pkgCache::PkgIterator &P, const pkgCacheFile &cacheFile, std::vector<std::string> tags, std::string version);
  std::string getString(napi_env env, napi_value value);
  std::vector<std::string> getArray(napi_env env, napi_value value);
  pkgRecords::Parser &LookupParser(pkgRecords &Recs, pkgCache::VerIterator const &V, pkgCache::VerFileIterator &Vf);
  napi_value init(napi_env env, napi_value exports);
  napi_value GetPackageCandidateVersion(napi_env env, napi_callback_info info);
  napi_value GetPackageCurrentVersion(napi_env env, napi_callback_info info);
  std::string getStringByChar(const char* symbol);
  
  napi_value GetPackageTags(napi_env env, napi_callback_info info) {
    size_t argc = 3;
    napi_value dictonary, second, args[3];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL); 
    if (status != napi_ok) return nullptr;
    
    std::string package_name;
    std::vector<std::string> tags;
    std::string version;
    try {
      package_name = getString(env, args[0]);
      tags = getArray(env, args[1]);
      try {
        version = getString(env, args[2]);
      } catch (...) {
        version = "current";
      }
    } catch (...) {
      return nullptr;
    }
    status = napi_create_object(env, &dictonary);
    if (status != napi_ok) return nullptr;

    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);
    pkgCacheFile cachefile;
    pkgCache *cache = cachefile.GetPkgCache();
    pkgCache::PkgIterator packageIterator = cache->FindPkg(package_name.c_str());
    
    if (packageIterator) {
      std::vector<std::pair<std::string, std::string>> dict = packageInfo(packageIterator, cachefile, tags, version);
      if (dict.size() == 0) return nullptr;
      
      napi_property_descriptor descriptors[dict.size()];
      
      for(std::vector<std::pair<std::string, std::string>>::iterator it = dict.begin(); it != dict.end(); ++it) {
        int index = it - dict.begin();
        status = napi_create_string_utf8(env, it->second.c_str(), it->second.length(), &second);
        if (status != napi_ok) return nullptr;
        
        napi_property_descriptor descriptor = { it->first.c_str(), NULL, NULL, NULL, NULL, second, static_cast<napi_property_attributes>(napi_writable | napi_enumerable | napi_configurable), NULL };
        descriptors[index] = descriptor;
      }

      status = napi_define_properties(env, dictonary, dict.size(), descriptors);

      if (status != napi_ok) return nullptr;
      return dictonary;
    } 
    return nullptr;
  }

  napi_value GetPackageCandidateVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value result, args[1];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL); 
    if (status != napi_ok) return nullptr;
    std::string package_name;
    try {
      package_name = getString(env, args[0]);
    } catch (...) {
      return nullptr;
    }
    pkgCacheFile cachefile;
    pkgCache *cache = cachefile.GetPkgCache();
    pkgCache::PkgIterator packageIterator = cache->FindPkg(package_name.c_str());
    if (packageIterator) {
      pkgCache::VerIterator cand = packageIterator.VersionList();
      if (cand) {
        std::string versionStr = getStringByChar(cand.VerStr());
        status = napi_create_string_utf8(env, cand.VerStr(), versionStr.length(), &result);
        if (status != napi_ok) return nullptr;
        return result;
      }
    }
    return nullptr;
  }

  napi_value GetPackageCurrentVersion(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value result, args[1];
    napi_status status;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL); 
    if (status != napi_ok) return nullptr;
    std::string package_name;
    try {
      package_name = getString(env, args[0]);
    } catch (...) {
      return nullptr;
    }
    pkgCacheFile cachefile;
    pkgCache *cache = cachefile.GetPkgCache();
    pkgCache::PkgIterator packageIterator = cache->FindPkg(package_name.c_str());
    if (packageIterator) {
      pkgCache::VerIterator current = packageIterator.CurrentVer();
      if (current) {
        std::string versionStr = getStringByChar(current.VerStr());
        status = napi_create_string_utf8(env, versionStr.c_str(), versionStr.length(), &result);
        if (status != napi_ok) return nullptr;
        return result;
      }
    }
    return nullptr;
  }

  std::vector<std::pair<std::string, std::string>> packageInfo(const pkgCache::PkgIterator &P, const pkgCacheFile &cacheFile, std::vector<std::string> tags, std::string version) {
    std::vector<std::pair<std::string, std::string>> result = {};
    pkgCache::VerIterator ver_iterator;
    if (version != "current") {
      pkgCache::VerIterator version_list = P.VersionList();
      while(!version_list.end()) {
        std::string current_version = getStringByChar(version_list.VerStr());
        if (current_version == version) {
          ver_iterator = version_list;
          break;
        };
        version_list++;
      }
    } else ver_iterator = P.CurrentVer();
    if(!ver_iterator || !ver_iterator.FileList()) return result;
    std::string current_version(ver_iterator.VerStr());
    pkgRecords Recs(cacheFile);
    pkgCache::VerFileIterator vf_iterator = ver_iterator.FileList();
    
    auto &Parser = LookupParser(Recs, ver_iterator, vf_iterator);
    char const *Start, *Stop;
    Parser.GetRec(Start, Stop);
    size_t const Length = Stop - Start + 1;
    pkgTagSection Tags;
    bool error = Tags.Scan(Start, Length, true);
    (void)error;
    
    for(std::vector<std::string>::iterator it = tags.begin(); it != tags.end(); ++it) {
      std::pair<std::string, std::string> record(*it, Tags.FindS((*it).c_str()));
      result.push_back(record);
    }
    return result;
  }

  std::string getStringByChar(const char* ver) {
      std::string tmp_string(ver);
      return tmp_string;
  }
  /*
  * N-API value to std::string
  */
  std::string getString(napi_env env, napi_value value) {
    napi_status status;
    size_t str_size;
    size_t str_size_read;
    status = napi_get_value_string_utf8(env, value, NULL, 0, &str_size);
    if (status != napi_ok) return nullptr;
    char * buf;
    buf = (char*)calloc(str_size + 1, sizeof(char));
    str_size = str_size + 1;
    napi_get_value_string_utf8(env, value, buf, str_size, &str_size_read);
    std::string package_name(buf);
    return package_name;
  }

  /**
  * N-API Array to std::vector
  */
  std::vector<std::string> getArray(napi_env env, napi_value value) {
    napi_status status;
    std::vector<std::string> result;
    uint32_t i, length;
    status = napi_get_array_length(env, value, &length);
    if (status != napi_ok) return result;
    
    for (i = 0; i < length; i++) {
      napi_value e;
      status = napi_get_element(env, value, i, &e);
      if (status != napi_ok) {
        result.clear();
        return result;
      }
      result.push_back(getString(env, e));
    }
    return result;
  } 

  pkgRecords::Parser &LookupParser(pkgRecords &Recs, pkgCache::VerIterator const &V, pkgCache::VerFileIterator &Vf) {
    Vf = V.FileList();
    for (; Vf.end() == false; ++Vf) {
      if ((Vf.File()->Flags & pkgCache::Flag::NotSource) == 0)
      break;
    }

    if (Vf.end() == true) Vf = V.FileList();
    return Recs.Lookup(Vf);
  }

  napi_value init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value fn1, fn2, fn3;
    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);    
    
    status = napi_create_function(env, nullptr, NAPI_AUTO_LENGTH, GetPackageTags, nullptr, &fn1);
    if (status != napi_ok) return nullptr;
    status = napi_create_function(env, nullptr, NAPI_AUTO_LENGTH, GetPackageCandidateVersion, nullptr, &fn2);
    if (status != napi_ok) return nullptr;
    status = napi_create_function(env, nullptr, NAPI_AUTO_LENGTH, GetPackageCurrentVersion, nullptr, &fn3);
    if (status != napi_ok) return nullptr;
    status = napi_set_named_property(env, exports, "getPackageTags", fn1);
    if (status != napi_ok) return nullptr;
    status = napi_set_named_property(env, exports, "getPackageCandidateVersion", fn2);
    if (status != napi_ok) return nullptr;
    status = napi_set_named_property(env, exports, "getPackageCurrentVersion", fn3);
    if (status != napi_ok) return nullptr;
    return exports;
  }


  NAPI_MODULE(debstep, init)
}