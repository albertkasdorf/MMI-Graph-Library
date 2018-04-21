#pragma once
#include <cstdint>
#include <vector>
#include <graph_iterator.h>

namespace graph
{
class edge;

class vertex
{
private:
	uint32_t _id;
	//
	// Edges that points to adjacent vertices.
	//
	std::vector<const edge*> _edges;
	//
	// Directed edges that points to this vertex.
	//
	std::vector<const edge*> _pointing_edges;

public:
	vertex(const uint32_t);
	vertex(const vertex&);
	~vertex();

public:
	//
	// Returns the id of this vertex.
	//
	uint32_t get_id(void) const;
	//
	// Changes the id of this vertex.
	//
	void set_id(uint32_t);
	//
	// Add a new edge to this vertex.
	//
	void add_edge(const edge*);
	//
	// Add a directed edge that points to this vertex.
	//
	void add_pointing_edge(const edge*);
	//
	// Remove an edge from this vertex.
	//
	void remove_edge(const edge*);
	//
	// Remove a directed edge that points to this vertex.
	//
	void remove_pointing_edge(const edge*);
	//
	// Returns the begin/end iterator to the edges.
	//
	std::pair<edge_iterator, edge_iterator> get_edges(void) const;
	//
	// Returns the begin/end iterator to the directed edges that points to this
	// vertex.
	//
	void get_pointing_edges(void) const;

public:
	static std::size_t create_hash(const uint32_t);
	static std::size_t create_hash(const vertex&);
};

}
