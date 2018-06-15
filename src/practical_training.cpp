#include <practical_training.h>

#include <iostream>
#include <chrono>
#include <functional>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>

#include <graph.h>
#include <graph_loader.h>
#include <graph_algorithm.h>
#include <graph_edge.h>
#include <graph_vertex.h>
#include <graph_comparer.h>
#include <graph_vertex_with_balance.h>
#include <graph_edge_with_cost_capacity.h>


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


//	std::shared_ptr<graph::vertex> v = std::make_shared<graph::vertex>(8045);
//	std::shared_ptr<const graph::vertex> cv = v;
//	v->set_id(45);
//	graph::vertex* pv = v.get();
//	const graph::vertex* pcv = cv.get();
//	const graph::vertex& rpc = *cv;

//	//graph::edge* pbla = nullptr;
//	//graph::edge& rbla = *pbla;


//	std::hash<std::string> hasher;
//	auto h1 = hasher("");
//	auto h3 = hasher("12-34");
//	auto h2 = hasher("34-12");

//	std::multimap<int, std::string> mm;

//	mm.insert(std::make_pair(45, "Hello"));
//	mm.insert(std::make_pair(45, "Hello2"));

//	auto count_of_45 = mm.count(45);

//	auto wtf = mm.find(45);
//	//for(auto element : mm.find(45))
//	//	continue;

//	std::stringstream ss;
//	ss << 45 << "-" << 67;
//	const std::string combined = ss.str();

//	graph::loader gl;
//	graph::graph gg_full, gg_res_bfs, gg_res_dfs;
//	graph::algorithm ga;
//	const graph::vertex* gv_start;

//	gl.load(graph::files::Graph1, gg_full);

//	gv_start = gg_full.get_vertex(0);
//	ga.breadth_first_search(&gg_full, gv_start, &gg_res_bfs);
//	ga.depth_first_search(&gg_full, gv_start, &gg_res_dfs);


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
	graph::files graph_file = graph::files::G_10_200;
	graph::graph g, t_with_prim, t_with_kruskal;
	graph::algorithm graph_algorithm;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	double cost_of_prim = 0.f, cost_of_kruskal = 0.f;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl;
	graph_loader.load(graph_file, g);

	start = std::chrono::high_resolution_clock::now();
	{
		const graph::vertex* start_vertex = g.get_vertex(0);
		graph_algorithm.prim(&g, start_vertex, &t_with_prim, &cost_of_prim);
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "=== Prim ===" << std::endl;
	std::cout << "Full graph<" << g.get_vertex_count() << "," << g.get_edge_count() << ">" << std::endl;
	std::cout << "Mst  graph<" << t_with_prim.get_vertex_count() << "," << t_with_prim.get_edge_count() << ">" << std::endl;
	std::cout << "Mst cost: " << cost_of_prim << std::endl;

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	{
		graph_algorithm.kruskal(&g, &t_with_kruskal, &cost_of_kruskal);
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << "=== Kruskal ===" << std::endl;
	std::cout << "Full graph<" << g.get_vertex_count() << "," << g.get_edge_count() << ">" << std::endl;
	std::cout << "Mst  graph<" << t_with_kruskal.get_vertex_count() << "," << t_with_kruskal.get_edge_count() << ">" << std::endl;
	std::cout << "Mst cost: " << cost_of_kruskal << std::endl;

	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << " ms" << std::endl;

	return;
}

//
// K_10.txt (38.41; 0.0026s, mit B&B 0,0018s)
// K_10e.txt (27.26; 0.0026s, mit B&B 0,0008s)
// K_12.txt (45.19; 0.25s, mit B&B 0,072s)
// K_12e.txt (36.13; 0.25s, mit B&B 0,013s)
//
void practical_training::task03_tsp(void)
{
	graph::loader graph_loader;
	graph::files graph_file = graph::files::K_10;
	graph::graph g, nn_hamilton_graph, dt_hamilton_graph, tar_hamilton_graph;
	const graph::vertex* start_vertex = nullptr;
	graph::algorithm graph_algorithm;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl;
	graph_loader.load(graph_file, g);
	start_vertex = g.get_vertex(0);

	start = std::chrono::high_resolution_clock::now();
	{
		graph_algorithm.nearest_neighbor(&g, start_vertex, &nn_hamilton_graph);
	}
	end = std::chrono::high_resolution_clock::now();
	print_tsp_result(
		std::string("Nearest neighbor"), &start, &end, &nn_hamilton_graph, start_vertex);

	start = std::chrono::high_resolution_clock::now();
	{
		graph_algorithm.double_tree(&g, start_vertex, &dt_hamilton_graph);
	}
	end = std::chrono::high_resolution_clock::now();
	print_tsp_result(
		std::string("Double tree"), &start, &end, &dt_hamilton_graph, start_vertex);

	start = std::chrono::high_resolution_clock::now();
	{
		graph_algorithm.try_all_routes(
			&g, start_vertex, false, &tar_hamilton_graph);
	}
	end = std::chrono::high_resolution_clock::now();
	print_tsp_result(
		std::string("try_all_routes"), &start, &end, &tar_hamilton_graph, start_vertex);

//	start = std::chrono::high_resolution_clock::now();
//	{
//		graph_algorithm.try_all_routes(
//			&g, start_vertex, true, &tar_hamilton_graph);
//	}
//	end = std::chrono::high_resolution_clock::now();
//	print_tsp_result(
//		std::string("try_all_routes with branch and bound"), &start, &end, &tar_hamilton_graph, start_vertex);

	return;
}

void practical_training::print_tsp_result(
	const std::string& algorithm_name,
	const std::chrono::time_point<std::chrono::high_resolution_clock>* start,
	const std::chrono::time_point<std::chrono::high_resolution_clock>* end,
	const graph::graph* graph,
	const graph::vertex* start_vertex)
{
	const graph::vertex* current_vertex = nullptr;
	std::queue<const graph::vertex*> processing_queue;
	std::set<const graph::vertex*, graph::compare_vertex_id> vertex_lookup;
	double trip_cost = 0.0;

	std::cout << "=== " << algorithm_name << " ===" << std::endl;
	std::cout << "Elapsed time: ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(*end - *start).count();
	std::cout << " ms" << std::endl;

	current_vertex = graph->get_vertex(start_vertex->get_id());
	processing_queue.push(current_vertex);
	vertex_lookup.insert(current_vertex);

	std::cout << "Trip: ";
	while(!processing_queue.empty())
	{
		current_vertex = processing_queue.front();
		processing_queue.pop();

		std::cout << current_vertex->get_id() << "->";

		for(auto edge : current_vertex->get_edges())
		{
			const graph::vertex* target_vertex = edge->get_target();
			const bool vertex_found = vertex_lookup.count(target_vertex) != 0;

			if(vertex_found)
				continue;

			processing_queue.push(target_vertex);
			vertex_lookup.insert(target_vertex);

			trip_cost += edge->get_weight();

			break;
		}
	}

	for(auto edge : current_vertex->get_edges())
	{
		if(edge->get_target()->get_id() == start_vertex->get_id())
		{
			std::cout << start_vertex->get_id() << std::endl;
			trip_cost += edge->get_weight();
			break;
		}
	}

	std::cout << "Cost: " << trip_cost << std::endl << std::endl;
}

//
// Zur Kontrolle:
// Bei gerichteter Betrachtungsweise hat der kürzeste Weg von Knoten 2 zu
// Knoten 0 bei Wege1 die Länge 6, bei Wege2 die Länge 2.
// Bei Wege3 gibt es einen negativen Zykel.
// Weitere Kontrollen:
// Bei G_1_2 hat der kürzeste Weg von 0 nach 1 die Länge 5.54417, wenn man die
// Kanten gerichtet versteht, bzw. 2.36796 im ungerichteten Fall.
//
// Wege1(2->0) = 6
// Wege2(2->0) = 2
// Wege3(2->0) = -
// G_1_2(0->1) = 5.54417
// G_1_2(0<->1) = 2.36796
//
void practical_training::task04_shortest_path(void)
{
	graph::loader graph_loader;
	const graph::files graph_file = graph::files::Wege1;
	const std::uint32_t start_vertex_id = 2;
	const bool create_directed_graph = true;
	graph::graph g, dijkstra_spt, mbf_spt;
	const graph::vertex* start_vertex = nullptr;
	graph::algorithm graph_algorithm;
	bool negative_weights_found = false;
	bool negative_cycle_found = false;
	std::map<const graph::vertex*, double> dijkstra_distance, mbf_distance;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl << std::endl;
	graph_loader.load(graph_file, g, create_directed_graph);
	start_vertex = g.get_vertex(start_vertex_id);

	std::cout << "=== Dijkstra ===" << std::endl;
	graph_algorithm.dijkstra(
		&g, start_vertex, &dijkstra_spt, &dijkstra_distance, &negative_weights_found);
	if(negative_weights_found)
	{
		std::cout << "=> Negative weights found." << std::endl;
	}
	print_shortest_path_result(start_vertex, &dijkstra_distance);

	std::cout << "=== Moore Bellman Ford ===" << std::endl;
	graph_algorithm.moore_bellman_ford(
		&g, start_vertex, &mbf_spt, &mbf_distance, &negative_cycle_found);
	if(negative_cycle_found)
	{
		std::cout << "=> Negative cycle found." << std::endl;
	}
	print_shortest_path_result(start_vertex, &mbf_distance);
}

void practical_training::print_shortest_path_result(
	const graph::vertex* start_vertex,
	const std::map<const graph::vertex*, double>* distance)
{
	for(auto kvp : *distance)
	{
		std::cout << "[" << start_vertex->get_id() << "->" << kvp.first->get_id() << "] = ";
		std::cout << kvp.second << " | ";
	}
	std::cout << std::endl << std::endl;
}

//
// Fluss(0->7) = 4
// Fluss2(0->7) = 5
// Fluss3(0->5) = 5
// G_1_2(0->7) = 0.735802
//
void practical_training::task05_maximum_flow(void)
{
	graph::loader graph_loader;
	const graph::files graph_file = graph::files::G_1_2;
	const std::uint32_t source_id = 0;
	const std::uint32_t target_id = 7;
	const bool create_directed_graph = true;
	graph::graph g;
	const graph::vertex* source_vertex = nullptr;
	const graph::vertex* target_vertex = nullptr;
	graph::algorithm graph_algorithm;
	double maximum_flow = 0.0;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl << std::endl;
	graph_loader.load(graph_file, g, create_directed_graph);
	source_vertex = g.get_vertex(source_id);
	target_vertex = g.get_vertex(target_id);

	std::cout << "=== Edmonds Karp ===" << std::endl;
	graph_algorithm.edmonds_karp(&g, source_vertex, target_vertex, &maximum_flow);
	std::cout << "Maximum flow between " << source_id << " and " << target_id;
	std::cout << " is " << maximum_flow << std::endl;

	return;
}

//
// In Klammern stehen die Kosten eines kostenminimalen Flusses.
// Kostenminimal1.txt (3)
// Kostenminimal2.txt (kein b-Fluss möglich)
// Kostenminimal3.txt (1537)
// Kostenminimal4.txt (0)
// Kostenminimal5.txt (0)
//
void practical_training::task06_minimum_cost_flow(void)
{
	graph::loader graph_loader;
	const graph::files graph_file = graph::files::Kostenminimal1;
	graph::graph full_graph;
	graph::algorithm algorithm;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	std::cout << "Loading graph file: ";
	std::cout << graph_loader.file_name_get(graph_file) << std::endl << std::endl;
	graph_loader.load(graph_file, full_graph);

	std::cout << "=== Cycle Cancelling ===" << std::endl;
	algorithm.cycle_cancelling(
		&full_graph, &minimum_cost_flow_found, &minimum_cost_flow);
	if(minimum_cost_flow_found)
		std::cout << "Minimal cost flow is " << minimum_cost_flow << std::endl;
	else
		std::cout << "Minimal cost flow not found." << std::endl;
}
