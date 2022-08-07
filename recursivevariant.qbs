import qbs

QtApplication {
    cpp.cxxLanguageVersion: "c++17"

    cpp.defines: [
        // You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]
    // cpp.cxxFlags: [""]

    consoleApplication: true
    files: [
        "fastpimpl.h",
        "main.cpp",
        "utils.h",
        "variant.cpp",
        "variant.h",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
