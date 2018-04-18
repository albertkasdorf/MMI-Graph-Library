#include <practical_training.h>

#include <iostream>
#include <chrono>
#include <functional>
#include <string>

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
//	graph::graph g;

//	g.add(graph::edge(1, 2));
//	g.add(graph::edge(1, 3));
//	g.add(graph::edge(2, 3));

//	g.remove(graph::edge(2, 3));


	/*
	 * graph
	 * - vertex_add(id) : void
	 * - vertex_remove(vertex) : void
	 * - vertex_get(void) : iterator_pair<std::shared_ptr<const vertex>>
	 * - edge_add(src, tgt, weight, direction);
	 * - edge_remove(edge) : void
	 * - edge_get(void) : iterator_pair<std::shared_ptr<const edge>>
	 *
	 * vertex
	 * - id_get(void) : id
	 * - edges_get(void) : iterator_pair<std::shared_ptr<const edge>>
	 *
	 * edge
	 * - vertex_get(src/tgt) : std::shared_ptr<const vertex>
	 * - twin_has() : bool
	 * - twin_get() : std::shared_ptr<const edge>
	 * - weight_get() : float
	 * - weight_has() : bool
	 *
	 * vertex_const_iterator
	 * edge_const_iterator
	 *
	 */


	std::shared_ptr<graph::vertex> v = std::make_shared<graph::vertex>(8045);
	std::shared_ptr<const graph::vertex> cv = v;
	v->id(45);
	graph::vertex* pv = v.get();
	const graph::vertex* pcv = cv.get();

	std::hash<std::string> hasher;
	auto h1 = hasher("");
	auto h3 = hasher("12-34");
	auto h2 = hasher("34-12");

	std::multimap<int, std::string> mm;

	mm.insert(std::make_pair(45, "Hello"));
	mm.insert(std::make_pair(45, "Hello2"));

	auto count_of_45 = mm.count(45);

	auto wtf = mm.find(45);
	//for(auto element : mm.find(45))
	//	continue;

	return;
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
	graph::files graph_file = graph::files::G_1_20;
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
