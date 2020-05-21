cmake_minimum_required(VERSION 3.10)
project( cppworkflow CXX )

option( ENABLED_CONSOLE  "The console features" OFF)
option( ENABLED_PARALLEL "The parallel features" ON)

include(cmake/Conan.cmake)
set(CMAKE_BUILD_TYPE Release)

add_library( cppfeatures INTERFACE)
target_compile_features(cppfeatures INTERFACE cxx_std_17)

include(cmake/warnings.cmake)
add_library( cppwarnings INTERFACE)
set_project_warinigs(cppwarnings)


include(cmake/Dependency.cmake)


add_library( cppworkflow INTERFACE )
target_link_libraries( cppworkflow INTERFACE cppfeatures cppworkflow )

add_subdirectory(src)

if ( ENABLED_PARALLEL )
  target_link_libraries( cppworkflow INTERFACE parallel )
endif()

add_subdirectory(examples)
