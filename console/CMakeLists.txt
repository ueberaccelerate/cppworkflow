project(console)

add_executable( cppconsole main.cpp)
target_link_libraries(cppconsole PRIVATE cppfeatures cppwarnings)
