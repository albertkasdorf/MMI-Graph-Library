cmake_minimum_required(VERSION 3.5.1)
project(graph_lib)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)
set(HEADERS
	include/graph_files.h
	include/graph_vertex.h
	include/graph_edge.h
	include/graph.h
	include/graph_files.h
	include/graph_loader.h
	include/graph_algorithm.h
	include/graph_iterator.h
	include/practical_training.h
	include/graph_comparer.h)
set(SOURCES
	src/main.cpp
	src/graph_algorithm.cpp
	src/graph.cpp
	src/graph_loader.cpp
	src/graph_iterator.cpp
	src/graph_edge.cpp
	src/practical_training.cpp
	src/graph_vertex.cpp
	src/graph_comparer.cpp)

add_custom_target(Headers SOURCES ${HEADERS})
add_executable(${PROJECT_NAME} ${SOURCES})
