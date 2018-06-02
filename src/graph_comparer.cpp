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

std::size_t undirected_edge_hash::operator()(const edge* edge) const
{
	const std::uint32_t source_id = edge->get_source()->get_id();
	const std::uint32_t target_id = edge->get_target()->get_id();

	std::stringstream ss;
	std::hash<std::string> h;

	if(source_id < target_id)
	{
		ss << source_id << "-" << target_id;
	}
	else
	{
		ss << target_id << "-" << source_id;
	}

	const std::size_t hash = h(ss.str());

	return hash;
}

//------------------------------------------------------------------------------

bool undirected_edge_equal::operator()(const edge* lhs, const edge* rhs) const
{
	const std::uint32_t lhs_source_id = lhs->get_source()->get_id();
	const std::uint32_t lhs_target_id = lhs->get_target()->get_id();
	const std::uint32_t rhs_source_id = rhs->get_source()->get_id();
	const std::uint32_t rhs_target_id = rhs->get_target()->get_id();

	const bool equal =
		(lhs_source_id == rhs_source_id) && (lhs_target_id == rhs_target_id);
	const bool flipped_equal =
		(lhs_source_id == rhs_target_id) && (lhs_target_id == rhs_source_id);

	return equal || flipped_equal;
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
