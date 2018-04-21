#include <graph_comparer.h>
#include <functional>
#include <sstream>
#include <string>
#include <graph_vertex.h>
#include <graph_edge.h>

namespace graph
{

//------------------------------------------------------------------------------

bool compare_vertex_id::operator()(const vertex* lhs, const vertex* rhs) const
{
	return lhs->get_id() > rhs->get_id();
}

bool compare_vertex_id::operator()(const vertex& lhs, const vertex& rhs) const
{
	return lhs.get_id() > rhs.get_id();
}

bool compare_vertex_id::operator()(
	const std::shared_ptr<vertex>& lhs,
	const std::shared_ptr<vertex>& rhs) const
{
	return lhs.get()->get_id() > rhs.get()->get_id();
}

//------------------------------------------------------------------------------

bool compare_edge_weight::operator()(const edge* lhs, const edge* rhs) const
{
	return lhs->get_weight() > rhs->get_weight();
}

bool compare_edge_weight::operator()(const edge& lhs, const edge& rhs) const
{
	return lhs.get_weight() > rhs.get_weight();
}

//------------------------------------------------------------------------------

//bool compare_edge_ids::operator()(const edge& lhs, const edge& rhs) const
//{
//	const std::uint64_t lhs_id =
//		(static_cast<std::uint64_t>(lhs.get_source()->get_id()) << 32) + lhs.get_target()->get_id();
//	const std::uint64_t rhs_id =
//		(static_cast<std::uint64_t>(rhs.get_source()->get_id()) << 32) + rhs.get_target()->get_id();

//	return lhs_id > rhs_id;
//}

//------------------------------------------------------------------------------

//
// Checks whether lhs is less than rhs.
//
//bool compare_edge_by_vertex_id::operator()(
//	const std::shared_ptr<edge>& lhs, const std::shared_ptr<edge>& rhs) const
//{
//	std::stringstream ss;
//	std::hash<std::string> hs;

//	ss.str(std::string());
//	if(lhs->get_source()->get_id() < lhs->get_target()->get_id())
//	{
//		ss << lhs->get_source()->get_id() << "-" << lhs->get_target()->get_id();
//	}
//	else
//	{
//		ss << lhs->get_target()->get_id() << "-" << lhs->get_source()->get_id();
//	}
//	const std::size_t lhs_hash = hs(ss.str());

//	ss.str(std::string());
//	if(rhs->get_source()->get_id() < rhs->get_target()->get_id())
//	{
//		ss << rhs->get_source()->get_id() << "-" << rhs->get_target()->get_id();
//	}
//	else
//	{
//		ss << rhs->get_target()->get_id() << "-" << rhs->get_source()->get_id();
//	}
//	const std::size_t rhs_hash = hs(ss.str());

//	return lhs < rhs;
//}

//------------------------------------------------------------------------------

}
