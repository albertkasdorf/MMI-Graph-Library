#pragma once

#include <set>
#include <map>
#include <cstdint>
#include <vector>
#include <graph_iterator.h>

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

public:
	//
	// Add a vertex to the graph.
	//
	void add(const vertex&);

	//
	// Add a new edge to the graph.
	// Remark:
	// - Vertices created if they not exist in graph.
	//
	void add(const edge&);

	//
	// Remove an edge from the graph.
	// Remark:
	// - Unconnected vertices are not removed.
	//
	void remove(const edge&);




	//
	// Return a copy of the vertex with the provided vertex id.
	//
	vertex vertex_get(std::uint32_t) const;


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
};

}
