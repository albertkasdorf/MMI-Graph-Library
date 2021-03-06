cmake_minimum_required(VERSION 3.5.1)
project(graph_lib)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
#
# No more leaks with sanitize flags in gcc and clang
# https://lemire.me/blog/2016/04/20/no-more-leaks-with-sanitize-flags-in-gcc-and-clang/
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(HEADERS
	include/graph_files.h
	include/graph_vertex.h
	include/graph_vertex_with_balance.h
	include/graph_edge.h
	include/graph_edge_with_cost_capacity.h
	include/graph.h
	include/graph_files.h
	include/graph_loader.h
	include/graph_algorithm.h
	include/graph_iterator.h
	include/practical_training.h
	include/graph_comparer.h)
set(SOURCES
	src/graph_algorithm.cpp
	src/graph.cpp
	src/graph_loader.cpp
	src/graph_iterator.cpp
	src/graph_edge.cpp
	src/graph_edge_with_cost_capacity.cpp
	src/practical_training.cpp
	src/graph_vertex.cpp
	src/graph_vertex_with_balance.cpp
	src/graph_comparer.cpp)
set(SOURCES_MAIN
	src/main.cpp)

set(SOURCES_TEST
	test/main.cpp
	test/shortest_path_test.cpp
	test/minimum_cost_flow_test.cpp
	test/graph_test.cpp)
set(FILE_NAME_TEST ${PROJECT_NAME}_test)

#
# Use gtest in ROS program
# https://ysonggit.github.io/coding/2014/12/19/use-gtest-in-ros-program.html
#
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Main executable
add_custom_target(Headers SOURCES ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES_MAIN} ${SOURCES})

# Test executable
add_executable(${FILE_NAME_TEST} ${SOURCES_TEST} ${SOURCES})
target_link_libraries(${FILE_NAME_TEST} ${GTEST_LIBRARIES} pthread)
