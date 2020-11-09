from conans import tools, ConanFile, CMake

class Pagoda(ConanFile):
    name = "pagoda"
    version = "0.1"
    description = "Graph-Based Procedural Modelling Framework"
    settings = "os", "compiler", "arch", "build_type"
    default_settings = (
        "build_type=Release"
    )
    options = {
        "shared" : [True, False],
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
        if self.settings.os == "Windows":
            tools.run_in_windows_bash(self, "cmake ..")
            tools.run_in_windows_bash(self, "cmake --build .")
        else:
            cmake = CMake(self, build_type=self.settings.build_type)
            cmake.configure()
            cmake.build()
