#include <node_api.h>
#include <apt-pkg/cachefile.h>
#include <apt-pkg/pkgcache.h>
#include <apt-pkg/pkgrecords.h>

using namespace std;

namespace demo {
    
  napi_value Method(napi_env env, napi_callback_info args) {
    napi_value greeting;
    napi_status status;

    pkgCacheFile cache_file;
    pkgCache* cache = cache_file.GetPkgCache();

    for (pkgCache::PkgIterator package = cache->PkgBegin(); !package.end(); package++) {
        std::cout << package.Name() << std::endl;
    }

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