project(threadpool)

add_executable( threadpool threadpool.cpp)
target_link_libraries(threadpool PRIVATE cppfeatures cppwarnings )
