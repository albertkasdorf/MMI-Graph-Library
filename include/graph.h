#pragma once

#include <set>
#include <map>
#include <cstdint>
#include <vector>

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
	// Return a copy of the vertex with the provided vertex id.
	//
	vertex vertex_get(std::uint32_t) const;

	//
	// Return the number of vertices.
	//
	std::uint32_t vertex_count_get(void) const;

	//
	// Add a new edge to the graph.
	// Remark:
	// - Vertices created if they not exist in graph.
	//
	void edge_add(const edge&);

	//
	// Return the edges of a given vertex.
	//
	const std::vector<edge> edge_get(const vertex&) const;
};

}
