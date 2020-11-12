from conans import tools, ConanFile, CMake

import os

class Pagoda(ConanFile):
    name = "pagoda"
    version = "0.1"
    description = "Graph-Based Procedural Modelling Framework"
    settings = "os", "compiler", "arch", "build_type"
    default_settings = (
        "build_type=Release"
    )
    options = {
        "shared" : [True, False]
    }
    default_options = (
        "shared=False"
    )
    generators = "cmake"

    requires = [
        ("boost/1.73.0"),
        ("gtest/1.10.0"),
        ("libpng/1.6.37"),
        ("nlohmann_json/3.9.1")
    ]

    def source(self):
        git = tools.Git()
        git.clone("https://github.com/diegoarjz/pagoda.git", branch="master")
        git.checkout("v{version}".format(version=Pagoda.version))


    def build(self):
        cmake = CMake(self, build_type=self.settings.build_type)

        # CI Definitions
        if 'GITHUB_ACTION' in os.environ:
            cmake.definitions['PAGODA_VERSION'] = os.environ['GITHUB_ACTION']
        if 'GITHUB_RUN_NUMBER' in os.environ:
            cmake.definitions['PAGODA_BUILD_NUMBER'] = os.environ['GITHUB_ACTION']

        cmake.configure(defs=cmake.definitions)
        cmake.build()

    def package(self):
        self.copy("bin/pagoda", "bin", keep_path=False)
        self.copy("lib/*", "lib", keep_path=False)
