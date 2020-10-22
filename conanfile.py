from conans import tools, ConanFile, CMake

class Pagoda(ConanFile):
    name = "pagoda"
    version = "0.1"
    description = "Graph-Based Procedural Modelling Framework"
    settings = "os", "compiler", "arch"
    options = {
        "shared" : [True, False],
        "build_type": ["Release", "Debug"]
    }
    default_options = (
        "shared=False",
        "build_type=Release"
    )
    generators = "cmake"

    requires = [
        ("boost/1.72.0"),
        ("gtest/1.10.0"),
        ("libpng/1.6.37"),
        ("nlohmann_json/3.9.1")
    ]


    def source(self):
        git = tools.Git()
        git.clone("https://github.com/diegoarjz/pagoda.git", branch="master")
        git.checkout("v{version}".format(version=Pagoda.version))


    def build(self):
        cmake = CMake(self, build_type=self.options.build_type)
        cmake.configure()
        cmake.build()
