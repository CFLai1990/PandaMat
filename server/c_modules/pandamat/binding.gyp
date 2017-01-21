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
        "operations/subspace.cc",
        "operations/coordinate.cc",
        "operations/coordinatews.cc",
        "operations/getcoordinates.cc",
        "operations/mds.cc",
        "operations/kmeans.cc",
      ],
      "include_dirs": ["operations"],
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