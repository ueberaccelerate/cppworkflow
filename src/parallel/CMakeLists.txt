project ( parallel )

add_library(parallel)
target_include_directories ( parallel INTERFACE include )


if( UNIX )
  target_link_libraries( parallel INTERFACE pthread )
endif()

target_sources( parallel PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parallel/queue.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parallel/stack.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parallel/threadpool.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/parallel/timer.hpp
  dump.cpp
)

add_subdirectory( container )
add_subdirectory( threadpool )
