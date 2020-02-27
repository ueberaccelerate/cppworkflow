cmake_minimum_required(VERSION 3.13)
project( cppworkflow CXX )

add_library( cppfeatures INTERFACE)
target_compile_features(cppfeatures INTERFACE cxx_std_17)

include(cmake/warnings.cmake)
add_library( cppwarnings INTERFACE)
set_project_warinigs(cppwarnings)

add_subdirectory(regex)
