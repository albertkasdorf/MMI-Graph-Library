#include <iostream>
#include <graph_loader.h>
#include <graph.h>
#include <graph_files.h>
#include <graph_algorithm.h>
#include <vertex.h>


int main()
{
	graph::loader gl;
	graph::graph g, t;
	graph::algorithm ga;

	gl.load(graph::files::Graph4, g);
	ga.breadth_first_search(g, graph::vertex(0), t);

	//g.edge_get(graph::vertex(0));

	std::cout << "Hello World!" << std::endl;
	return 0;
}
