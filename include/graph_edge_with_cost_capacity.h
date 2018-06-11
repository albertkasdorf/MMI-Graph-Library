#pragma once
#include <graph_edge.h>

namespace graph {

class edge_with_cost_capacity : public edge
{
private:
	double _cost;
	double _capacity;

public:
	edge_with_cost_capacity(const double, const double);
	edge_with_cost_capacity(edge_with_cost_capacity&) = delete;
	virtual ~edge_with_cost_capacity(void);

public:
	double get_cost(void) const;
	void set_cost(const double);

	double get_capacity(void) const;
	void set_capacity(const double);
};

}
