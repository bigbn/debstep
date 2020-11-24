#include <node_api.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>

using namespace std;

namespace demo {
  static pkgPolicy *policy;
  pkgRecords::Parser &LookupParser(pkgRecords &Recs, pkgCache::VerIterator const &V, pkgCache::VerFileIterator &Vf) /*{{{*/
  {
    Vf = V.FileList();
    for (; Vf.end() == false; ++Vf)
        if ((Vf.File()->Flags & pkgCache::Flag::NotSource) == 0)
    break;
    if (Vf.end() == true)
        Vf = V.FileList();
    return Recs.Lookup(Vf);
  }
  static void coutPackage(const pkgCache::PkgIterator &P, const pkgCacheFile &cacheFile)
  {
    pkgCache::VerIterator ver_iterator = P.CurrentVer();
    if(!ver_iterator || !ver_iterator.FileList())
        return;
    pkgRecords Recs(cacheFile);
    pkgCache::VerFileIterator vf_iterator = ver_iterator.FileList();
    auto &Parser = LookupParser(Recs, ver_iterator, vf_iterator);
    char const *Start, *Stop;
    Parser.GetRec(Start, Stop);
    size_t const Length = Stop - Start;
    pkgTagSection Tags;
    Tags.Scan(Start, Length, true);
    std::string key = "XB-Nginx-Template";
    std::cout << Tags.FindS("Package") << std::endl;
    std::cout << SizeToStr(Tags.FindULL("Installed-Size") * 1024).c_str() << std::endl;
  }
  napi_value Method(napi_env env, napi_callback_info args) {
    
    napi_value greeting;
    napi_status status;
    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);

    pkgCacheFile cachefile;
    pkgCache *cache = cachefile.GetPkgCache();
    policy = cachefile.GetPolicy();
    pkgCache::Header* head = cache->HeaderP;
    if (cache == NULL || _error->PendingError()) {
        _error->DumpErrors();
        return nullptr;
    }

    for (pkgCache::GrpIterator grp = cache->GrpBegin(); grp != cache->GrpEnd(); grp++)
       for (pkgCache::PkgIterator p = grp.PackageList(); !p.end(); p = grp.NextPkg(p))
          coutPackage(p, cachefile);
    status = napi_create_string_utf8(env, "world", NAPI_AUTO_LENGTH, &greeting);
    if (status != napi_ok) return nullptr;
    return greeting;
  }

  napi_value init(napi_env env, napi_value exports) {
    napi_status status;
    napi_value fn;
  
    pkgInitConfig(*_config);
    pkgInitSystem(*_config, _system);    

    status = napi_create_function(env, nullptr, 0, Method, nullptr, &fn);
    if (status != napi_ok) return nullptr;

    status = napi_set_named_property(env, exports, "hello", fn);
    if (status != napi_ok) return nullptr;
    return exports;
  }

  NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace demo