import os, re

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import load, copy, rmdir

required_conan_version = ">=2.2.0"

class ArbaStrnRecipe(ConanFile):
    project_namespace = "arba"
    project_base_name = "strn"
    name = f"{project_namespace}-{project_base_name}"
    package_type = "library"

    # Optional metadata
    description = "A C++ library providing short trivially comparable string classes whose hashs are trivially computable."
    url = "https://github.com/arapelle/arba-strn"
    homepage = "https://github.com/arapelle/arba-strn"
    topics = ("string")
    license = "MIT"
    author = "Aymeric Pellé"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "test": [True, False]
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "test": False
    }

    # Build
    win_bash = os.environ.get('MSYSTEM', None) is not None
    no_copy_source = True

    # Sources
    exports_sources = "LICENSE.md", "CMakeLists.txt", "test/*", "include/*", "src/*", "external/*", "cmake/*"

    # Other
    implements = ["auto_shared_fpic"]

    def set_version(self):
        cmakelist_content = load(self, os.path.join(self.recipe_folder, "CMakeLists.txt"))
        version_regex = r"""set_project_semantic_version\( *"?([0-9]+\.[0-9]+\.[0-9]+).*"""
        self.version = re.search(version_regex, cmakelist_content).group(1)

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 20)

    def requirements(self):
        self.requires("arba-vrsn/[^0.4]", transitive_headers=True, transitive_libs=True)

    def build_requirements(self):
        self.test_requires("gtest/[^1.14]")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        upper_name = f"{self.project_namespace}_{self.project_base_name}".upper()
        tc.variables[f"{upper_name}_LIBRARY_TYPE"] = "SHARED" if self.options.shared else "STATIC"
        if self.options.test:
            tc.variables[f"BUILD_{upper_name}_TESTS"] = "TRUE"
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.options.test:
            cmake.ctest(cli_args=["--progress", "--output-on-failure"])

    def package(self):
        copy(self, "LICENSE.md", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))

    def package_info(self):
        postfix = "" if self.options.shared else "-static"
        name = self.name + postfix
        self.cpp_info.set_property("cmake_target_name", name.replace('-', '::', 1))
        if self.settings.build_type == "Debug":
            name += "-d"
        self.cpp_info.libs = [name]
