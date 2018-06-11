#include "graph_edge_with_cost_capacity.h"

namespace graph {

edge_with_cost_capacity::edge_with_cost_capacity(
	const double cost,
	const double capacity)
	:
	edge(), _cost(cost), _capacity(capacity)
{
}

edge_with_cost_capacity::~edge_with_cost_capacity(void)
{
}

double edge_with_cost_capacity::get_cost(void) const
{
	return _cost;
}

void edge_with_cost_capacity::set_cost(const double value)
{
	_cost = value;
}

double edge_with_cost_capacity::get_capacity(void) const
{
	return _capacity;
}

void edge_with_cost_capacity::set_capacity(const double value)
{
	_capacity = value;
}

}

