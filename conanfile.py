from conans import tools, ConanFile, CMake

import os

class Pagoda(ConanFile):
    name = "pagoda"
    description = "Graph-Based Procedural Modelling Framework"
    version = "0.1"
    license = "MIT"
    author = "Diego Jesus <diego.a.r.jz@gmail.com>"
    url = "https://github.com/diegoarjz/pagoda"
    topics = ("procedural modelling", "geometry", "content generation")

    settings = "os", "compiler", "arch", "build_type"
    default_settings = (
        "build_type=Release"
    )
    options = {
        "shared" : [True, False],
    }
    default_options = (
        "shared=False",
        "qt:shared=True",
        "glew:shared=False"
    )
    generators = "cmake_find_package"
    exports_sources = "*"


    def requirements(self):
        self.requires("boost/1.73.0")
        self.requires("gtest/1.10.0")
        self.requires("libpng/1.6.37")
        self.requires("nlohmann_json/3.9.1")
        self.requires("openssl/1.1.1k", override=True)
        self.requires("zlib/1.2.12", override=True)
        self.requires("qt/6.1.1")
        self.requires("entt/3.6.0")
        self.requires("glew/2.1.0")


    def _configure_cmake(self):
        cmake = CMake(self, build_type=self.settings.build_type)

        # CI Definitions
        cmake.definitions['PAGODA_VERSION'] = self.version
        if 'GITHUB_RUN_NUMBER' in os.environ:
            cmake.definitions['PAGODA_BUILD_NUMBER'] = os.environ['GITHUB_RUN_NUMBER']

        if self.settings.build_type == 'Release':
            cmake.definitions['CMAKE_BUILD_TYPE'] = 'Release'
        elif self.settings.build_type == 'Debug':
            cmake.definitions['CMAKE_BUILD_TYPE'] = 'Debug'

        cmake.configure(defs=cmake.definitions)
        return cmake


    def build(self):
        cmake = self._configure_cmake()
        cmake.build()


    def imports(self):
        self.copy("lib/*Qt6OpenGLWidgets*", src="", dst="src/bin", root_package="qt", keep_path=False)
        self.copy("lib/*Qt6Widgets*", src="", dst="src/bin", root_package="qt", keep_path=False)
        self.copy("lib/*Qt6OpenGL*", src="", dst="src/bin", root_package="qt", keep_path=False)
        self.copy("lib/*Qt6Gui*", src="", dst="src/bin", root_package="qt", keep_path=False)
        self.copy("lib/*Qt6Core*", src="", dst="src/bin", root_package="qt", keep_path=False)

        self.copy("res/archdatadir/plugins/platforms/*.dylib", src="", dst="src/bin/platforms", root_package="qt", keep_path=False)
        self.copy("res/archdatadir/plugins/platforms/*.so", src="", dst="src/bin/platforms", root_package="qt", keep_path=False)
        self.copy("res/archdatadir/plugins/platforms/*.dll", src="", dst="src/bin/platforms", root_package="qt", keep_path=False)


    def package(self):
        cmake = CMake(self, build_type=self.settings.build_type)
        cmake.install()

        self.copy("src/bin/pagoda",             "bin", keep_path=False)
        self.copy("src/bin/pgeditor",           "bin", keep_path=False)
        self.copy("src/bin/platforms/*.dylib",  "bin/platforms", keep_path=False)
        self.copy("src/bin/platforms/*.dll",    "bin/platforms", keep_path=False)
        self.copy("src/bin/platforms/*.so",     "bin/platforms", keep_path=False)
        self.copy("src/bin/*.dylib",            "bin", keep_path=False)
        self.copy("src/bin/*.dll",              "bin", keep_path=False)
        self.copy("src/bin/*.so",               "bin", keep_path=False)
        self.copy("src/bin/pagoda.exe",         "bin", keep_path=False)
        self.copy("src/bin/pgeditor.exe",       "bin", keep_path=False)
        self.copy("src/pagoda/libpagoda.a",     "lib", keep_path=False)
        self.copy("src/pgeditor/libpgeditor.a", "lib", keep_path=False)


    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.libs = ["pagoda"]
        self.cpp_info.bindirs = ["bin"]
