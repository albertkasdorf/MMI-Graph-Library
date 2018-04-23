#pragma once

#include <set>
#include <map>
#include <cstdint>
#include <vector>
#include <memory>
#include <graph_iterator.h>
#include <graph_comparer.h>

namespace graph
{
class vertex;
class edge;

class graph
{
public:
	graph();
	~graph();

private:
	std::map<std::size_t, std::shared_ptr<vertex>> vertices;
	std::multimap<std::size_t, std::shared_ptr<edge>> edges;

public:
	//
	// Add a vertex to the graph.
	//
	void add_vertex(const uint32_t);

	//
	// Add a new edge to the graph.
	// Remark:
	// - Vertices created if they not exist in graph.
	//
	void add_edge(const edge*);
	//void add_edge(const edge&);
	void add_undirected_edge(
		const uint32_t&, const uint32_t&);
	void add_undirected_edge(
		const uint32_t&, const uint32_t&, const double&);
	void add_undirected_edge(
		const uint32_t&, const uint32_t&, const double*);

	//
	// Get all edges of the graph.
	//
	std::pair<edge_iterator_on_multimap, edge_iterator_on_multimap> get_edges(void) const;


	//
	// Remove an edge from the graph.
	// Remark:
	// - Unconnected vertices are not removed.
	//
	void remove_edge(const edge&);




	//
	// Return a copy of the vertex with the provided vertex id.
	//
	const vertex* get_vertex(const std::uint32_t) const;
	//
	// Return the number of vertices.
	//
	std::uint32_t get_vertex_count(void) const;
	//
	// Return all vertices of the graph.
	//
	std::pair<vertex_iterator, vertex_iterator> get_vertices(void) const;

	//
	// Returns the number of edges.
	//
	std::uint32_t get_edge_count(void) const;





private:
	vertex* get_vertex_internal(const std::uint32_t) const;
};

}
