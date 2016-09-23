{
  "targets": [
    {
      "target_name": "pandamat",
      "sources": [
        "addon.cc", "pandamat.cc", "pandapool.cc",
        "operations/operationdata.cc",
        "opercollection.cc", "operations/operation.cc",
        "operations/add.cc",
        "operations/dot.cc", 
        "operations/transpose.cc",
        "operations/normalize.cc", 
        "operations/sqrtdist.cc",
      ],
      "include_dirs": ["/opt/armadillo-7.400.2-gcc6.2.0/include/", "operations"],
      "link_settings": {
        "libraries": [
          "-larmadillo",
        ],
        "ldflags": [
            "-L /opt/armadillo-7.400.2-gcc6.2.0/lib",
            "-Wl,-rpath, /opt/armadillo-7.400.2-gcc6.2.0/lib/libarmadillo.so.7",
        ]
      },
      'cflags!': [ '-fno-exceptions', '-fopenmp', '-Wall'],
      'cflags_cc!': [ '-fno-exceptions', '-fopenmp', '-Wall'],
      'configurations': {
        'Release': {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': 1,
            }
          }
        }
      },
    }
  ]
}