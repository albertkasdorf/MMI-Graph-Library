#pragma once
#include <graph_vertex.h>

namespace graph
{

class vertex_with_balance : public vertex
{
private:
	double _balance;

public:
	vertex_with_balance(const uint32_t, const double);
	vertex_with_balance(const vertex_with_balance&);
	virtual ~vertex_with_balance(void);

public:
	double get_balance(void) const;
	void set_balance(const double);
};

}
