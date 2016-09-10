{
  "targets": [
    {
      "target_name": "pandamat",
      "sources": [ "addon.cc", "pandamat.cc", 
      "opercollection.cc", "operation.cc", 
      "add.cc", "dot.cc", "transpose.cc"],
      "include_dirs": ["/opt/armadillo-7.400.2/include/"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
    }
  ]
}