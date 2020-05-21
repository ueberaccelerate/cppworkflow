project ( parallel )

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(parallel INTERFACE)
target_include_directories ( parallel INTERFACE include )

if( UNIX )
  target_link_libraries( parallel INTERFACE pthread )
endif()

add_subdirectory( container )
add_subdirectory( threadpool )
