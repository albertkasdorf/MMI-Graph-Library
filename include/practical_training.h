#pragma once
#include <chrono>
#include <string>
#include <graph.h>
#include <graph_vertex.h>

class practical_training
{
public:
	practical_training();
	~practical_training();

public:
	void task00_debugging(void);
	void task01_bfs_dfs(void);
	void task02_prim_kruskal(void);
	void task03_tsp(void);
	void task04_shortest_path(void);

private:
	void print_tsp_result(
		const std::string&,
		const std::chrono::time_point<std::chrono::high_resolution_clock>*,
		const std::chrono::time_point<std::chrono::high_resolution_clock>*,
		const graph::graph*,
		const graph::vertex*);

	void print_shortest_path_result(
		const graph::vertex*,
		const std::map<const graph::vertex*, double>*);
};
