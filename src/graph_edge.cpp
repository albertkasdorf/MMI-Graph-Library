#include <graph_edge.h>
#include <graph_vertex.h>

namespace graph
{

edge::edge(
	const std::uint32_t source_id,
	const std::uint32_t target_id,
	const float weight,
	const bool directed)
	:
	_source_id(source_id),
	_target_id(target_id),
	_weight(weight),
	_directed(directed)
{
}

edge::edge(const edge& rhs) :
	_source_id(rhs.source_id()),
	_target_id(rhs.target_id()),
	_weight(rhs.weight()),
	_directed(rhs.directed())
{
}

edge::~edge()
{
}


std::uint32_t edge::source_id(void) const
{
	return _source_id;
}

/*
 * edge: 6 -> 7
 *
 * pov: 6
 * - source_id: 6
 * - target_id: 7
 *
 * pov: 7
 * - source_id: 7
 * - target_id: 6
 *
 */
std::uint32_t edge::source_id(const vertex& point_of_view) const
{
	if( _source_id == point_of_view.id())
	{
		return _source_id;
	}
	else
	{
		return _target_id;
	}
}

std::uint32_t edge::target_id(void) const
{
	return _target_id;
}

std::uint32_t edge::target_id(const vertex& point_of_view) const
{
	if( _target_id == point_of_view.id())
	{
		return _source_id;
	}
	else
	{
		return _target_id;
	}
}

float edge::weight(void) const
{
	return _weight;
}

bool edge::directed(void) const
{
	return _directed;
}

}
