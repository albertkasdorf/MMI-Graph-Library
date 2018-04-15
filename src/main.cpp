#include <iostream>
#include <vector>
#include <memory>
#include <graph_loader.h>
#include <graph.h>
#include <graph_files.h>
#include <graph_algorithm.h>
#include <graph_vertex.h>


int main()
{
	graph::loader gl;
	graph::graph g, t_bfs, t_dfs;
	graph::algorithm ga;
	std::vector<std::shared_ptr<graph::graph>> subgraphs;
	graph::vertex vertex_start(7);

	gl.load(graph::files::Graph4, g);
	//ga.breadth_first_search(g, vertex_start, t_bfs);
	//ga.depth_first_search(g, vertex_start, t_dfs);


	ga.connected_component(g, subgraphs);


	std::cout << "Hello World!" << std::endl;
	return 0;
}
