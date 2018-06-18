#pragma once
#include <cstdint>
#include <vector>
#include <graph_iterator.h>
#include <memory>

namespace graph
{
class edge;

class vertex
{
private:
	uint32_t _id;

	//
	// Balance of this node.
	//
	std::unique_ptr<double> _balance;

	//
	// Edges that points to adjacent vertices.
	//
	std::vector<const edge*> _edges;

	//
	// Directed edges that points to this vertex.
	//
	//std::vector<const edge*> _pointing_edges;

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
	// Check, Get and Set the Balance.
	//
	bool has_balance(void) const;
	double get_balance(void) const;
	void set_balance(double);

	//
	// Add a new edge to this vertex.
	//
	void add_edge(const edge*);

	//
	// Add a directed edge that points to this vertex.
	//
	//void add_pointing_edge(const edge*);

	//
	// Remove an edge from this vertex.
	//
	void remove_edge(const edge*);

	//
	// Remove a directed edge that points to this vertex.
	//
	//void remove_pointing_edge(const edge*);

	//
	// Returns the begin/end iterator to the edges.
	//
	template<typename E = edge>
	std::pair<edge_iterator<E>, edge_iterator<E>> get_edges(void) const
	{
		return std::make_pair(
			edge_iterator<E>(_edges.cbegin()), edge_iterator<E>(_edges.cend()));
	}

	//
	// Returns the begin/end iterator to the directed edges that points to this
	// vertex.
	//
	//void get_pointing_edges(void) const;

	std::shared_ptr<vertex> create_copy(void) const;

public:
	static std::size_t create_hash(const uint32_t);
	static std::size_t create_hash(const vertex&);
};

}
