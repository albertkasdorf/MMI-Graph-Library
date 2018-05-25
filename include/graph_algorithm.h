#pragma once
#include <memory>
#include <vector>
#include <set>
#include <functional>
#include <stack>
#include <map>

namespace graph
{
class graph;
class vertex;
class edge;
class compare_vertex_id;

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
};

}
