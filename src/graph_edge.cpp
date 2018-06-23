#include <graph_edge.h>
#include <cassert>
#include <functional>
#include <string>
#include <sstream>
#include <graph_vertex.h>
#include <iostream>
#include <memory>

namespace graph
{

edge::edge()
{
	_source = nullptr;
	_target = nullptr;
	_twin = nullptr;
}

edge::~edge()
{
	_source = nullptr;
	_target = nullptr;
	_twin = nullptr;

	_weight.reset(nullptr);
	_cost.reset(nullptr);
	_capacity.reset(nullptr);
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
	return _weight != nullptr;
}

double edge::get_weight(void) const
{
	assert(has_weight());
	return *_weight;
}

void edge::set_weight(const double value)
{
	_weight.reset(new double(value));
}

inline bool edge::has_cost(void) const
{
	return _cost != nullptr;
}

inline double edge::get_cost(void) const
{
	assert(has_cost());
	return *_cost;
}

void edge::set_cost(const double value)
{
	_cost.reset(new double(value));
}

bool edge::has_capacity(void) const
{
	return _capacity != nullptr;
}

double edge::get_capacity(void) const
{
	assert(has_capacity());
	return *_capacity;
}

void edge::set_capacity(const double value)
{
	_capacity.reset(new double(value));
}

std::shared_ptr<edge> edge::create_copy(void) const
{
	std::shared_ptr<edge> e = std::make_shared<edge>();

	if(has_weight())
		e->set_weight(get_weight());
	if(has_capacity())
		e->set_capacity(get_capacity());
	if(has_cost())
		e->set_cost(get_cost());

	return e;
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
