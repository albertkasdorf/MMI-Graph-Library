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
	std::map<std::uint32_t, vertex*> vertices;
	std::vector<edge*> edges;
	std::map<std::uint32_t, std::map<std::uint32_t, std::vector<edge*>>> edges_map;

	std::map<std::size_t, std::shared_ptr<vertex>> vertices2;
	std::multimap<std::size_t, std::shared_ptr<edge>> edges2;

public:
	//
	// Add a vertex to the graph.
	//
	void vertex_add(const uint32_t);

	//
	// Add a new edge to the graph.
	// Remark:
	// - Vertices created if they not exist in graph.
	//
	void edge_add(const edge&);
	void edge_undirected_add(
		const uint32_t&, const uint32_t&);
	void edge_undirected_add(
		const uint32_t&, const uint32_t&, const float&);
	void edge_undirected_add(
		const uint32_t&, const uint32_t&, const float*);


	//
	// Remove an edge from the graph.
	// Remark:
	// - Unconnected vertices are not removed.
	//
	void remove(const edge&);




	//
	// Return a copy of the vertex with the provided vertex id.
	//
	const vertex* vertex_get(const std::uint32_t) const;


	//
	// Return the number of vertices.
	//
	std::uint32_t vertex_count_get(void) const;

	vertex_iterator vertices_begin() const;
	vertex_iterator vertices_end() const;



	//
	// Return the edges of a given vertex.
	//
	const std::vector<edge> edge_get(const vertex&) const;
	std::vector<edge> edge_get(void) const;

private:
	vertex* vertex_get_internal(const std::uint32_t) const;
};

}
