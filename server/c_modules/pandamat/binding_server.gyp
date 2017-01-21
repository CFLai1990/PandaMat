{
  "targets": [
    {
      "target_name": "pandamat",
      "sources": [
        "addon.cc", 
        "pandamat.cc", 
        "pandapool.cc",
        "opercollection.cc",
      ],
      "include_dirs": [
        "src/pm_Common/",
        "src/pm_Library/basic/",
        "src/pm_Library/",
        "src/FocusChanger/",
        "src/SMap/",
        "/var/home/chufan.lai/library/LDA/",
        "/var/home/chufan.lai/library/KNN/flann-1.8.4-src/src/cpp/",
        "/var/home/chufan.lai/eigen3.2.10/",
        "/var/home/chufan.lai/library/Projection/tapkee/include/",
        "/opt/armadillo-7.400.2-gcc6.2.0/include/",
        "/opt/mlpack-2.0.3/include/",
        "/opt/boost_1_53_0/include/",
      ],
      "link_settings": {
        "libraries": [
          "-lpm_library",
          "-lpm_common",
          "-lpm_focuschanger",
          "-lpm_smap",
          "-larmadillo",
          "-lglda",
          "-lboost_system",
          "-lboost_filesystem",
        ],
        "ldflags": [
            "-L /var/www/html/PandaMat/server/c_modules/pandamat/lib",
            "-L /opt/armadillo-7.400.2-gcc6.2.0/lib",
            "-L /var/home/chufan.lai/library/LDA/GibbsLDA++-0.2",
            "-L /opt/boost_1_53_0/lib",
        ]
      },
      'cflags!': [ 
        '-fno-exceptions', 
        '-fopenmp', 
        '-Wall', 
        '-fno-rtti',   
        "-std=c++11",
        "-std=gnu++0x",
        "-stdlib=libc++",
      ],
      'cflags_cc!': [ '-fno-exceptions', '-fopenmp', '-Wall', '-fno-rtti'],
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