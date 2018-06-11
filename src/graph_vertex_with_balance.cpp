#include "graph_vertex_with_balance.h"
#include <graph_vertex.h>

namespace graph
{

vertex_with_balance::vertex_with_balance( const uint32_t id, const double balance)
	:
	vertex(id), _balance(balance)
{
}

vertex_with_balance::vertex_with_balance(const vertex_with_balance& rhs)
	:
	vertex(rhs), _balance(rhs.get_balance())
{
}

vertex_with_balance::~vertex_with_balance(void)
{
}

double vertex_with_balance::get_balance(void) const
{
	return this->_balance;
}

void vertex_with_balance::set_balance(const double balance)
{
	this->_balance = balance;
}

}

