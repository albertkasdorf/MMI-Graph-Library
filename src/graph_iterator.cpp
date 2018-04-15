#include <graph_iterator.h>
#include <graph_vertex.h>

namespace graph
{

vertex_iterator::vertex_iterator(
	std::map<std::uint32_t, vertex*>::const_iterator iter)
	:
	iter(iter)
{
}

vertex_iterator::reference vertex_iterator::operator*() const
{
	return *((*iter).second);
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

}
