project( algorithms )

add_library( algorithms )
target_include_directories(algorithms PUBLIC include)

target_sources( algorithms PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/cppworkflow/algorithms/sort.hpp
  dump.cpp
)
