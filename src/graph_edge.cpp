#include <graph_edge.h>
#include <cassert>
#include <functional>
#include <string>
#include <sstream>
#include <graph_vertex.h>

namespace graph
{

edge::edge()
{
	_source = nullptr;
	_target = nullptr;
	_twin = nullptr;
	_has_weight = false;
	_weight = 0.0;
}

edge::~edge()
{
}

const vertex* edge::get_source(void) const
{
	return _source;
}

const vertex* edge::get_target(void) const
{
	return _target;
}

void edge::set_source(const vertex* new_vertex)
{
	_source = new_vertex;
}

void edge::set_target(const vertex* new_vertex)
{
	_target = new_vertex;
}

bool edge::has_twin(void) const
{
	return (_twin != nullptr);
}

bool edge::is_directed(void) const
{
	return has_twin();
}

const edge* edge::get_twin(void) const
{
	return _twin;
}

void edge::set_twin(const edge* new_twin)
{
	_twin = new_twin;
}

bool edge::has_weight(void) const
{
	return _has_weight;
}

double edge::get_weight(void) const
{
	assert(has_weight());
	return _weight;
}

void edge::set_weight(const double new_weight)
{
	_has_weight = true;
	_weight = new_weight;
}

std::size_t edge::get_hash(void) const
{
	return create_hash(this);
}

std::size_t edge::create_hash(const edge* e)
{
	std::stringstream ss;
	std::hash<std::string> h;

	if(e->get_source()->get_id() < e->get_target()->get_id())
	{
		ss << e->get_source()->get_id() << "-" << e->get_target()->get_id();
	}
	else
	{
		ss << e->get_target()->get_id() << "-" << e->get_source()->get_id();
	}

	const std::size_t hash = h(ss.str());

	return hash;
}

}
