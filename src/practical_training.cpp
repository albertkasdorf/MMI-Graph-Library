#include <practical_training.h>

#include <iostream>
#include <chrono>

#include <graph.h>
#include <graph_loader.h>
#include <graph_algorithm.h>
#include <graph_edge.h>
#include <graph_vertex.h>


practical_training::practical_training()
{
}

practical_training::~practical_training()
{
}

void practical_training::task00_debugging(void)
{
}

void practical_training::task01_bfs_dfs(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::Graph4;
	graph::graph g;
	graph::algorithm ga;
	std::vector<std::shared_ptr<graph::graph>> subgraphs_bfs, subgraphs_dfs;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl;
	graph_loader.load(graph_file, g);

	ga.connected_component_with_bfs(g, subgraphs_bfs);
	std::cout << "Connected components found with bfs: ";
	std::cout << subgraphs_bfs.size() << std::endl;

	ga.connected_component_with_dfs(g, subgraphs_dfs);
	std::cout << "Connected components found with dfs: ";
	std::cout << subgraphs_dfs.size() << std::endl;
}

void practical_training::task02_prim_kruskal(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::G_1_2;
	graph::graph g, t;
	graph::algorithm graph_algorithm;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	start = std::chrono::high_resolution_clock::now();
	{
		graph_loader.load(graph_file, g);
		//graph_algorithm.prim(g, graph::vertex(0), t);
		graph_algorithm.kruskal(g, t);
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	return;
}
