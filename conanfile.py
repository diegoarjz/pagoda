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
        "glew:shared=False"
    )
    generators = "cmake"
    exports_sources = "*"

    requires = [
        ("boost/1.73.0"),
        ("gtest/1.10.0"),
        ("libpng/1.6.37"),
        ("nlohmann_json/3.9.1"),
        ("magnum/2020.06@pagoda/stable"),
        ("magnum-integration/2020.06@pagoda/stable"),
        ("corrade/2020.06@helmesjo/stable"),
        ("glew/2.1.0@bincrafters/stable"),
        ("glfw/3.3.2@bincrafters/stable"),
        ("opengl/system")
    ]


    def build(self):
        cmake = CMake(self, build_type=self.settings.build_type)

        # CI Definitions
        cmake.definitions['PAGODA_VERSION'] = self.version
        if 'GITHUB_RUN_NUMBER' in os.environ:
            cmake.definitions['PAGODA_BUILD_NUMBER'] = os.environ['GITHUB_RUN_NUMBER']

        magnum_integration_modules = os.path.join(self.deps_cpp_info["magnum-integration"].rootpath , "source_subfolder", "modules")
        cmake.definitions["MagnumIntegrationModules"] = magnum_integration_modules
        imgui_dir = os.path.join(self.deps_cpp_info["magnum-integration"].rootpath, "imgui_subfolder")
        cmake.definitions["IMGUI_DIR"] = imgui_dir

        cmake.configure(defs=cmake.definitions)
        cmake.build()


    def package(self):
        self.copy("bin/pagoda", "bin", keep_path=False)
        self.copy("bin/pagoda.exe", "bin", keep_path=False)
        self.copy("lib/*", "lib", keep_path=False)


    def package_info(self):
        self.cpp_info.libs = ["pagoda"]
