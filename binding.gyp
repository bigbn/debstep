{
  "variables" : {
    "openssl_fips": "",
  },
  "targets": [
    {
      "target_name": "bindings",
      "sources": [ "bindings.cpp" ],
      "library_dirs": [ "/usr/include/"],
      "cflags_cc": ['-fexceptions'],
      "libraries": [ "-lapt-pkg" ]      
    }
  ]
}