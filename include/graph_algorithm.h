#pragma once
#include <memory>
#include <vector>
#include <set>
#include <functional>
#include <stack>
#include <map>
#include <unordered_map>
#include <list>

namespace graph
{
class graph;
class vertex;
class edge;
class compare_vertex_id;
struct undirected_edge_hash;
struct undirected_edge_equal;

class algorithm
{
public:
	algorithm();
	~algorithm();

public:
	//
	// Compute and return the spanning tree of a graph from a vertex starting point.
	//
	void breadth_first_search(
		const graph*, const vertex*, graph*);

	//
	// Compute and return the spanning tree of a graph from a vertex starting point.
	//
	void depth_first_search(
		const graph*, const vertex*, graph*);

	//
	// Compute the connected components of a graph und returning all subgraphs.
	// Using the breadth first search spanning tree algorithm.
	//
	void connected_component_with_bfs(
		const graph*, std::vector<std::shared_ptr<graph>>*);

	//
	// Compute the connected components of a graph und returning all subgraphs.
	// Using the depth first search spanning tree algorithm.
	//
	void connected_component_with_dfs(
		const graph*, std::vector<std::shared_ptr<graph>>*);

	//
	// Find the minimal spanning tree with the prim algorithm.
	//
	void prim(const graph*, const vertex*, graph*, double*);

	//
	// Find the minimal spanning tree with the kruskal algorithm.
	//
	void kruskal(const graph*, graph*, double*);

	//
	// Nearest neighbor
	//
	void nearest_neighbor(const graph*, const vertex*, graph*);

	//
	// Double tree algorithm
	//
	void double_tree(const graph*, const vertex*, graph*);

	//
	// Try all possible routes of the graph and return the cheapest route.
	//
	void try_all_routes(const graph*, const vertex*, const bool, graph*);

	//
	// Dijkstra-Algorithm
	//
	void dijkstra(
		const graph*,
		const vertex*,
		graph*,
		std::map<const vertex*, double>*,
		bool*);

	//
	// Moore-Bellman-Ford-Algorithm
	//
	void moore_bellman_ford(
		const graph*,
		const vertex*,
		graph*,
		std::map<const vertex*, double>*,
		bool*);

	//
	// Edmonds-Karp
	//
	void edmonds_karp(const graph*, const vertex*, const vertex*, double*);
	void edmonds_karp(
		const graph*,
		const vertex*,
		const vertex*,
		std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>*,
		double*,
		std::function<double(const edge*)>);

	//
	// Cycle Cancelling Algorithm
	//
	void cycle_cancelling(const graph*, bool*, double*);

	//
	// Successive Shortest Path Algorithm
	//
	void successive_shortest_path(const graph*, bool*, double*);

private:
	void depth_first_search_recursive(
		const graph*,
		const vertex*,
		std::set<const vertex*, compare_vertex_id>*,
		graph* );

	//
	// Compute the connected components of a graph und returning all subgraphs.
	//
	void connected_component(
		const graph*,
		std::vector<std::shared_ptr<graph>>*,
		const std::function<void(const graph*, const vertex*, graph*)>&);

	//
	//
	//
	void try_all_routes_recursive(
		const vertex* const,
		const bool* const,
		const std::size_t* const,
		const vertex*,
		const double,
		std::stack<const edge*>* const,
		std::set<const vertex*>* const,
		std::stack<const edge*>* const,
		double* const);

	//
	//
	//
	void create_residual_graph(
		const graph*,
		const std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>*,
		graph*,
		std::function<double(const edge*)> capacity_of_edge);

	//
	//
	//
	void get_shortest_path(
		const graph*,
		const vertex*,
		const vertex*,
		std::list<const edge*>*,
		double*,
		std::function<double(const edge*)> capacity_of_edge);

	//
	//
	//
	void cc_compute_b_flow(
		const graph* full_graph,
		bool* b_flow_exist,
		std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>* flow_per_edge);

	//
	//
	//
	void cc_create_residual_graph(
		const graph* full_graph,
		const std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>* flow_per_edge,
		graph* residual_graph);

	//
	//
	//
	void cc_find_negative_cycle(
		const graph* residual_graph,
		const std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>* flow_per_edge,
		std::list<const edge*>* cycle,
		double* gamma);

	//
	//
	//
	void ssp_moore_bellman_ford(
		const graph* graph,
		const vertex* start_vertex,
		std::map<const vertex*, const edge*, compare_vertex_id>* predecessor);

	//
	//
	//
	void viz_flow(
		const bool show_viz,
		const std::unordered_map<
			const edge*,
			double,
			undirected_edge_hash,
			undirected_edge_equal>* flow_per_edge);

	//
	//
	//
	void viz_graph(
		const bool show_viz,
		const char* graph_type,
		const graph* graph);

	//
	//
	//
	void viz_pseudo_source_target(
		const bool show_viz,
		const std::vector<const vertex*>* pseudo_source,
		const std::vector<const vertex*>* pseudo_target);

	//
	//
	//
	void viz_flow_change(
		const bool show_viz,
		const std::list<std::pair<const edge*, bool>>* path_of_change,
		const double gamma);
};

}
