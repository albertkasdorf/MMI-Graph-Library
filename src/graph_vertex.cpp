#include <graph_vertex.h>
#include <functional>
#include <graph_edge.h>
#include <iostream>

namespace graph
{

vertex::vertex(const uint32_t id) :
	_id(id)
{
}

vertex::vertex(const vertex& rhs)
{
	_id = rhs._id;
	_edges = rhs._edges;

	if(rhs.has_balance())
		set_balance(rhs.get_balance());
}

vertex::~vertex()
{
	_id = 0;
	_edges.clear();
	_balance.reset(nullptr);
}

uint32_t vertex::get_id(void) const
{
	return _id;
}

void vertex::set_id(uint32_t id)
{
	_id = id;
}

bool vertex::has_balance(void) const
{
	return _balance != nullptr;
}

double vertex::get_balance(void) const
{
	assert(has_balance());
	return *_balance;
}

void vertex::set_balance(double value)
{
	_balance = std::make_unique<double>(value);
}

void vertex::add_edge(const edge* new_edge)
{
	_edges.push_back(new_edge);
}

std::shared_ptr<vertex> vertex::create_copy(void) const
{
	std::shared_ptr<vertex> bla;

	bla = std::make_shared<vertex>(_id);

	if(has_balance())
		bla->set_balance(get_balance());

	return bla;
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
