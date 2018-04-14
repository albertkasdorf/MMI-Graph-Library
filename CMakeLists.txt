cmake_minimum_required(VERSION 2.8)
project(graph_lib)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(HEADERS
	include/graph_files.h
	include/vertex.h
	include/edge.h
	include/graph.h
	include/graph_files.h
	include/graph_loader.h
	include/graph_algorithm.h
	include/comparer.h)
set(SOURCES
	src/main.cpp
	src/graph_algorithm.cpp
	src/graph.cpp
	src/graph_loader.cpp)

add_custom_target(Headers SOURCES ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})
