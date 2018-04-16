#pragma once

#include <graph_vertex.h>
#include <graph_edge.h>

namespace graph
{

struct compare_vertex_id
{
	// Return smallest first
	bool operator()(const vertex* lhs, const vertex* rhs) const
	{
		return lhs->id() > rhs->id();
	}

	bool operator()(const vertex& lhs, const vertex& rhs) const
	{
		return lhs.id() > rhs.id();
	}
};

struct compare_edge_weight
{
	// Return smallest first
	bool operator()(const edge* lhs, const edge* rhs) const
	{
		return lhs->weight() > rhs->weight();
	}

	bool operator()(const edge& lhs, const edge& rhs) const
	{
		return lhs.weight() > rhs.weight();
	}
};

struct compare_edge_ids
{
	bool operator()(const edge& lhs, const edge& rhs) const
	{
		const std::uint64_t lhs_id =
			(static_cast<std::uint64_t>(lhs.source_id()) << 32) + lhs.target_id();
		const std::uint64_t rhs_id =
			(static_cast<std::uint64_t>(rhs.source_id()) << 32) + rhs.target_id();

		return lhs_id > rhs_id;
	}
};

}
