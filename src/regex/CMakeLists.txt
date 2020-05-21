project(regex)

add_executable( cppregex main.cpp)
target_link_libraries(cppregex PRIVATE cppfeatures cppwarnings CONAN_PKG::imgui-sfml)
