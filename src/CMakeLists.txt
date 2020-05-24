
#info( "Enabled parallel: ${ENABLED_PARALLEL}" )
add_subdirectory(algorithms)
if( ENABLED_PARALLEL ) 

  add_subdirectory( parallel )

endif()
