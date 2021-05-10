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
        "boost:without_fiber=True",
        "glew:shared=False"
    )
    generators = "cmake"
    exports_sources = "*"


    def requirements(self):
        self.requires("boost/1.73.0")
        self.requires("gtest/1.10.0")
        self.requires("libpng/1.6.37")
        self.requires("nlohmann_json/3.9.1")
        self.requires("openssl/1.1.1k", override=True)
        self.requires("qt/6.0.1@bincrafters/stable")
        self.requires("entt/3.6.0")
        self.requires("glew/2.1.0@bincrafters/stable")


    def build(self):
        cmake = CMake(self, build_type=self.settings.build_type)

        # CI Definitions
        cmake.definitions['PAGODA_VERSION'] = self.version
        if 'GITHUB_RUN_NUMBER' in os.environ:
            cmake.definitions['PAGODA_BUILD_NUMBER'] = os.environ['GITHUB_RUN_NUMBER']

        cmake.configure(defs=cmake.definitions)
        cmake.build()


    def imports(self):
        self.copy("*.dylib", src="", dst="bin", root_package="qt", keep_path=False)


    def package(self):
        self.copy("bin/pagoda", "bin", keep_path=False)
        self.copy("bin/pagoda.exe", "bin", keep_path=False)
        self.copy("lib/*", "lib", keep_path=False)


    def package_info(self):
        self.cpp_info.libs = ["pagoda"]
