#include <graph_vertex.h>
#include <functional>
#include <graph_edge.h>

namespace graph
{

vertex::vertex(const uint32_t id) :
	_id(id)
{
}

// TODO: Do I really need a copy ctor?
vertex::vertex(const vertex& rhs) :
	_id(rhs.get_id())
	, _edges(rhs._edges)
	, _pointing_edges(rhs._pointing_edges)
{
}

vertex::~vertex()
{
}

uint32_t vertex::get_id(void) const
{
	return _id;
}

void vertex::set_id(uint32_t id)
{
	_id = id;
}

void vertex::add_edge(const edge* new_edge)
{
	_edges.push_back(new_edge);
}

void vertex::add_pointing_edge(const edge* new_edge)
{
	_pointing_edges.push_back(new_edge);
}

std::pair<edge_iterator, edge_iterator> vertex::get_edges(void) const
{
	return std::make_pair(
		edge_iterator(_edges.cbegin()), edge_iterator(_edges.cend()));
}

std::size_t vertex::create_hash(const std::uint32_t id)
{
	std::hash<std::uint32_t> h;
	const std::size_t hash = h(id);
	return hash;
}

std::size_t vertex::create_hash(const vertex& v)
{
	return create_hash(v.get_id());
}



}
