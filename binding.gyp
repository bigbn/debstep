{
  "targets": [
    {
      "target_name": "bindings",
      "sources": [ "bindings.cpp" ],
      "library_dirs": [ "/usr/include/"],
      "cflags_cc": ['-fpermissive', '-fexceptions'],
      "libraries": [ "-lapt-pkg" ]      
    }
  ]
}