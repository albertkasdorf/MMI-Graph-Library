#include <practical_training.h>
#include <iostream>
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
	//ga.breadth_first_search(g, vertex_start, t_bfs);
	//ga.depth_first_search(g, vertex_start, t_dfs);
}

void practical_training::task01_bfs_dfs(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::Graph4;

	graph::graph g, t_bfs, t_dfs;
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
