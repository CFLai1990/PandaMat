{
  "targets": [
    {
      "target_name": "pandamat",
      "sources": [ "addon.cc", "pandamat.cc", 
      "opercollection.cc", "operation.cc", 
      "add.cc", "dot.cc", "transpose.cc"],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
    }
  ]
}