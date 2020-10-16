from conans import ConanFile, CMake

class Pagoda(ConanFile):
    name = "pagoda"
    version = "0.1"
    settings = "os", "compiler", "arch"
    options = {"shared" : [True, False]}
    default_options = "shared=False"
    generators = "cmake"

    requires = [
        ("boost/1.72.0"),
        ("gtest/1.10.0"),
        ("libpng/1.6.37"),
        ("nlohmann_json/3.9.1")
    ]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
