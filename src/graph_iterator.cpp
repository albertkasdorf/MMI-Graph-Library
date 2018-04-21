#include <graph_iterator.h>
#include <graph_vertex.h>
#include <graph_edge.h>

namespace graph
{

vertex_iterator::vertex_iterator(
	std::map<std::size_t, std::shared_ptr<vertex>>::const_iterator iter)
	:
	iter(iter)
{
}

vertex_iterator::pointer vertex_iterator::operator*() const
{
	return (*iter).second.get();
}

vertex_iterator& vertex_iterator::operator++()
{
	++iter;
	return *this;
}

vertex_iterator vertex_iterator::operator++(int)
{
	vertex_iterator tmp = *this;
	++iter;
	return tmp;
}

// Equality: it == end().
bool vertex_iterator::operator==(const vertex_iterator& rhs)
{
	return iter == rhs.iter;
}

// Inequality: it != end().
bool vertex_iterator::operator!=(const vertex_iterator& rhs)
{
	return iter != rhs.iter;
}


edge_iterator::edge_iterator(
	std::vector<const edge*>::const_iterator iter) :  iter(iter)
{
}

edge_iterator::pointer edge_iterator::operator*() const
{
	return (*iter);
}

edge_iterator& edge_iterator::operator++()
{
	++iter;
	return *this;
}

edge_iterator edge_iterator::operator++(int)
{
	edge_iterator tmp = *this;
	++iter;
	return tmp;
}

// Equality: it == end().
bool edge_iterator::operator==(const edge_iterator& rhs)
{
	return iter == rhs.iter;
}

// Inequality: it != end().
bool edge_iterator::operator!=(const edge_iterator& rhs)
{
	return iter != rhs.iter;
}

}
