from conans import ConanFile, CMake, tools


class AnnotatedSentenceConan(ConanFile):
    name = "AnnotatedSentence-C"
    version = "1.0.0"
    requires = ["DependencyParser-C/1.0.0", "NamedEntityRecognition-C/1.0.0", "PropBank-C/1.0.0", "FrameNet-C/1.0.0", "SentiNet-C/1.0.0"]
    license = "GPL License"
    author = "Olcay Taner Yildiz olcay.yildiz@ozyegin.edu.tr"
    url = "https://github.com/StarlangSoftware/AnnotatedSentence-C"
    description = "Annotated Sentence Processing library"
    topics = ("")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"
    exports_sources = "src/*", "Test/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*AnnotatedSentence.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["AnnotatedSentence"]
