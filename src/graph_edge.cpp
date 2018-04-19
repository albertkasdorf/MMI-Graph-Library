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
	_weight = 0.f;
}

edge::~edge()
{
}

const vertex* edge::source(void) const
{
	return _source;
}

const vertex* edge::target(void) const
{
	return _target;
}

void edge::source(const vertex* new_vertex)
{
	_source = new_vertex;
}

void edge::target(const vertex* new_vertex)
{
	_target = new_vertex;
}

bool edge::twin_has(void) const
{
	return (_twin != nullptr);
}

bool edge::directed(void) const
{
	return twin_has();
}

const edge* edge::twin(void) const
{
	return _twin;
}

void edge::twin(const edge* new_twin)
{
	_twin = new_twin;
}

bool edge::weight_has(void) const
{
	return _has_weight;
}

float edge::weight(void) const
{
	assert(weight_has());
	return _weight;
}

void edge::weight(const float new_weight)
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

	if(e->source()->id() < e->target()->id())
	{
		ss << e->source()->id() << "-" << e->target()->id();
	}
	else
	{
		ss << e->target()->id() << "-" << e->source()->id();
	}

	const std::size_t hash = h(ss.str());

	return hash;
}

}
