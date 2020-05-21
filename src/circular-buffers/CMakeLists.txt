project(circular-buffers)

add_executable( cppcircular-buffers-raw rawarraycircular.cpp)
target_link_libraries(cppcircular-buffers-raw PRIVATE cppfeatures cppwarnings )
