#pragma once

#include <vertex.h>
#include <edge.h>

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
};

}
