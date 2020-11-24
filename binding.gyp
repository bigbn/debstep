{
  "targets": [
    {
      "target_name": "bindings",
      "sources": [ "bindings.cpp" ],
      "library_dirs": [ "/usr/include/"],
      "cflags_cc!": [ "-fno-rtti" ],
      "cflags!": [ "-fno-rtti" ],
      "libraries": [ "-lapt-pkg" ]      
    }
  ]
}