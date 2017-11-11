{
    "targets": [
        {
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ],
            "cflags_cc": [
               "-O3"
            ],
            "xcode_settings": {
                "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
                "CLANG_CXX_LIBRARY": "libc++",
                "MACOSX_DEPLOYMENT_TARGET": "10.7",
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "GCC_ENABLE_CPP_RTTI": "YES",
                "OTHER_CPLUSPLUSFLAGS": [
                  
                ]
            },
            "libraries": [
                "-lwebp",
                "-lpng",
                "-ljpeg"
            ],
            "target_name": "addon",
            "sources": [
                "main.cpp",
                "encode_worker.cpp",
                "dec/imageio_util.c",
                "dec/jpegdec.c",
                "dec/metadata.c",
                "dec/pngdec.c"
            ]
        }
    ]
}