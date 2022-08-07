import qbs

Project {
    StaticLibrary {
        name: "lib"
        Depends { name: "Qt.core" }
        cpp.cxxLanguageVersion: "c++17"
        files: [
            "fastpimpl.h",
            "utils.h",
            "variant.cpp",
            "variant.h",
        ]
    }
    QtApplication {
        name: "example"
        Depends { name: "lib" }

        cpp.cxxLanguageVersion: "c++17"

        consoleApplication: true
        files: "main.cpp"

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
            qbs.installDir: "bin"
        }
    }

    QtApplication {
        Depends { name: "Qt.test" }
        name: "test_variant"
        cpp.cxxLanguageVersion: "c++17"
        consoleApplication: true
        files: "test_variant.cpp"
    }
}
