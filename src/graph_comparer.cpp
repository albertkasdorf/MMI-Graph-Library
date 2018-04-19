#include <graph_comparer.h>
#include <functional>
#include <sstream>
#include <string>
#include <graph_vertex.h>
#include <graph_edge.h>

namespace graph
{

bool compare_vertex_id::operator()(const vertex* lhs, const vertex* rhs) const
{
	return lhs->id() > rhs->id();
}

bool compare_vertex_id::operator()(const vertex& lhs, const vertex& rhs) const
{
	return lhs.id() > rhs.id();
}

bool compare_vertex_id::operator()(
	const std::shared_ptr<vertex>& lhs,
	const std::shared_ptr<vertex>& rhs) const
{
	return lhs.get()->id() > rhs.get()->id();
}


bool compare_edge_weight::operator()(const edge* lhs, const edge* rhs) const
{
	return lhs->weight() > rhs->weight();
}

bool compare_edge_weight::operator()(const edge& lhs, const edge& rhs) const
{
	return lhs.weight() > rhs.weight();
}


bool compare_edge_ids::operator()(const edge& lhs, const edge& rhs) const
{
	const std::uint64_t lhs_id =
		(static_cast<std::uint64_t>(lhs.source()->id()) << 32) + lhs.target()->id();
	const std::uint64_t rhs_id =
		(static_cast<std::uint64_t>(rhs.source()->id()) << 32) + rhs.target()->id();

	return lhs_id > rhs_id;
}

//
// Checks whether lhs is less than rhs.
//
bool compare_edge_by_vertex_id::operator()(
	const std::shared_ptr<edge>& lhs, const std::shared_ptr<edge>& rhs) const
{
	std::stringstream ss;
	std::hash<std::string> hs;

	ss.str(std::string());
	if(lhs->source()->id() < lhs->target()->id())
	{
		ss << lhs->source()->id() << "-" << lhs->target()->id();
	}
	else
	{
		ss << lhs->target()->id() << "-" << lhs->source()->id();
	}
	const std::size_t lhs_hash = hs(ss.str());

	ss.str(std::string());
	if(rhs->source()->id() < rhs->target()->id())
	{
		ss << rhs->source()->id() << "-" << rhs->target()->id();
	}
	else
	{
		ss << rhs->target()->id() << "-" << rhs->source()->id();
	}
	const std::size_t rhs_hash = hs(ss.str());

	return lhs < rhs;
}

}
