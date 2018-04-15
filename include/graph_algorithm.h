#pragma once
#include <memory>
#include <vector>
#include <set>

namespace graph
{
class graph;
class vertex;
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
		const graph&, const vertex&, graph&);

	//
	// Compute and return the spanning tree of a graph from a vertex starting point.
	//
	void depth_first_search(
		const graph&, const vertex&, graph&);

	//
	// Compute the connected components of a graph und returning all subgraphs.
	//
	void connected_component(
		const graph&, std::vector<std::shared_ptr<graph>>&);

private:
	void depth_first_search(
		const graph&,
		const vertex&,
		std::set<vertex, compare_vertex_id>&,
		graph& );
};

}
