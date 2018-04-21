#include <practical_training.h>

#include <iostream>
#include <chrono>
#include <functional>
#include <string>
#include <sstream>

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
	 *	- kante duplizieren
	 *	- Vertices raussuchen
	 *	- Kante bei den vertices eintragen
	 *	- Verticies bei der Kante eintragen
	 * - edge_remove(edge) : void
	 *	- Kante aus den Vertices entfernen
	 *	- vertices entfernen, falls leer
	 * - edge_get(void) : iterator_pair<std::shared_ptr<const edge>>
	 *
	 * vertex
	 * - id_get(void) : id
	 * - edges_get(void) : iterator_pair<std::shared_ptr<const edge>>
	 *
	 * edge
	 * + vertex_get(src/tgt) : std::shared_ptr<const vertex>
	 * + twin_has() : bool
	 * + twin_get() : std::shared_ptr<const edge>
	 * - weight_get() : float
	 * - weight_has() : bool
	 *
	 * vertex_const_iterator
	 * edge_const_iterator
	 *
	 */


	std::shared_ptr<graph::vertex> v = std::make_shared<graph::vertex>(8045);
	std::shared_ptr<const graph::vertex> cv = v;
	v->set_id(45);
	graph::vertex* pv = v.get();
	const graph::vertex* pcv = cv.get();
	const graph::vertex& rpc = *cv;

	//graph::edge* pbla = nullptr;
	//graph::edge& rbla = *pbla;


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

	std::stringstream ss;
	ss << 45 << "-" << 67;
	const std::string combined = ss.str();

	graph::loader gl;
	graph::graph gg_full, gg_res_bfs, gg_res_dfs;
	graph::algorithm ga;
	const graph::vertex* gv_start;

	gl.load(graph::files::Graph1, gg_full);

	gv_start = gg_full.get_vertex(0);
	ga.breadth_first_search(&gg_full, gv_start, &gg_res_bfs);
	ga.depth_first_search(&gg_full, gv_start, &gg_res_dfs);


	return;
}

void practical_training::task01_bfs_dfs(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::Graph4;
	graph::graph g;
	graph::algorithm ga;
	std::vector<std::shared_ptr<graph::graph>> subgraphs_bfs, subgraphs_dfs;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl;
	graph_loader.load(graph_file, g);

	start = std::chrono::high_resolution_clock::now();
	{
		ga.connected_component_with_bfs(&g, &subgraphs_bfs);
		std::cout << "Connected components found with bfs: ";
		std::cout << subgraphs_bfs.size() << std::endl;
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	{
		ga.connected_component_with_dfs(&g, &subgraphs_dfs);
		std::cout << "Connected components found with dfs: ";
		std::cout << subgraphs_dfs.size() << std::endl;
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;
}

void practical_training::task02_prim_kruskal(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::G_1_2;
	graph::graph g, t_with_prim, t_with_kruskal;
	graph::algorithm graph_algorithm;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl;
	graph_loader.load(graph_file, g);

	start = std::chrono::high_resolution_clock::now();
	{
		const graph::vertex* start_vertex = g.get_vertex(0);
		graph_algorithm.prim(&g, start_vertex, &t_with_prim);
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "=== Prim ===" << std::endl;
	std::cout << "Full graph<" << g.get_vertex_count() << "," << g.get_edge_count() << ">" << std::endl;
	std::cout << "Mst  graph<" << t_with_prim.get_vertex_count() << "," << t_with_prim.get_edge_count() << ">" << std::endl;

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	{
		graph_algorithm.kruskal(&g, &t_with_kruskal);
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "=== Kruskal ===" << std::endl;
	std::cout << "Full graph<" << g.get_vertex_count() << "," << g.get_edge_count() << ">" << std::endl;
	std::cout << "Mst  graph<" << t_with_kruskal.get_vertex_count() << "," << t_with_kruskal.get_edge_count() << ">" << std::endl;

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	return;
}
