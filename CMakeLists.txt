cmake_minimum_required(VERSION 3.10)
project( cppworkflow CXX )

include(cmake/Conan.cmake)

add_library( cppfeatures INTERFACE)
target_compile_features(cppfeatures INTERFACE cxx_std_17)

include(cmake/warnings.cmake)
add_library( cppwarnings INTERFACE)
set_project_warinigs(cppwarnings)
set(CONAN_EXTRA_REQUIRES ${CONAN_EXTRA_REQUIRES}
                         imgui-sfml/2.1@bincrafters/stable)
run_conan()

add_subdirectory(regex)
add_subdirectory(console)
