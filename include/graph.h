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
	graph(const graph&);
	~graph();

private:
	std::map<std::size_t, std::shared_ptr<vertex>> vertices;
	std::multimap<std::size_t, std::shared_ptr<edge>> edges;

public:
	//
	// Add a vertex to the graph.
	//
	const vertex* add_vertex(const uint32_t);

	//
	// Add a balanced vertex to the graph.
	//
	void add_vertex(const uint32_t, const double);

	//
	//
	//
	const vertex* add_vertex(const uint32_t*, const double*);

	//
	//
	//
	const vertex* add_vertex(const vertex*);

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
	// Add a new directed edge to the graph.
	// Remark:
	// - Vertices created if they not exist in graph.
	//
	void add_directed_edge(
		const uint32_t&, const uint32_t&, const double&);
	void add_directed_edge(
		const uint32_t&, const uint32_t&, const double*);

	//
	// Add a new directed edge to the graph. (With cost and capacity)
	//
	void add_directed_edge(
		const uint32_t, const uint32_t, const double, const double);

	//
	// Get all edges of the graph.
	//
	template<typename E = edge>
	std::pair<edge_iterator_on_multimap<E>, edge_iterator_on_multimap<E>> get_edges(void) const
	{
		return std::make_pair(
			edge_iterator_on_multimap<E>(edges.cbegin()),
			edge_iterator_on_multimap<E>(edges.cend()));
	}

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
	template<typename V = vertex>
	std::pair<vertex_iterator<V>, vertex_iterator<V>> get_vertices(void) const
	{
		return std::make_pair(
			vertex_iterator<V>(vertices.cbegin()),
			vertex_iterator<V>(vertices.cend()));
	}

	//
	// Returns the number of edges.
	//
	std::uint32_t get_edge_count(void) const;

	//
	// Returns the edge of the two vertices.
	//
	const edge* get_edge(const vertex*, const vertex*) const;

	//
	//
	//
	const edge* get_edge(const edge*) const;


private:
	vertex* get_vertex_internal(const std::uint32_t) const;
};

}
