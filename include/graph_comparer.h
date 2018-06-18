#pragma once
#include <memory>

namespace graph
{
class vertex;
class edge;

//------------------------------------------------------------------------------

struct compare_vertex_id
{
	// Return smallest first
	bool operator()(const vertex*, const vertex*) const;
	bool operator()(const vertex&, const vertex&) const;
	bool operator()(
		const std::shared_ptr<vertex>&, const std::shared_ptr<vertex>&) const;
};

//------------------------------------------------------------------------------

struct compare_edge_weight
{
	// Return smallest first
	bool operator()(const edge*, const edge*) const;
	bool operator()(const edge&, const edge&) const;
};

//------------------------------------------------------------------------------

struct undirected_edge_hash
{
	std::size_t operator()(const edge* edge) const;
};

//------------------------------------------------------------------------------

struct directed_edge_hash
{
	std::size_t operator()(const edge* edge) const;
};

//------------------------------------------------------------------------------

struct undirected_edge_equal
{
	bool operator()(const edge* lhs, const edge* rhs) const;
};

//------------------------------------------------------------------------------

struct directed_edge_equal
{
	bool operator()(const edge* lhs, const edge* rhs) const;
};

//------------------------------------------------------------------------------

//struct compare_edge_ids
//{
//	bool operator()(const edge& lhs, const edge& rhs) const;
//};

//------------------------------------------------------------------------------

//struct compare_edge_by_vertex_id
//{
//	bool operator()(
//		const std::shared_ptr<edge>&, const std::shared_ptr<edge>&) const;
//};

//------------------------------------------------------------------------------

}
