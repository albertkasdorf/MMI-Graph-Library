#include <graph.h>

#include <vertex.h>
#include <edge.h>
#include <comparer.h>

namespace graph
{

graph::graph()
{
}

graph::~graph()
{
	for(const auto& vertex : vertices)
		delete vertex.second;
	for(const auto& edge : edges)
		delete edge;
}

vertex graph::vertex_get(std::uint32_t id) const
{
	return *(vertices.at(id));
}

std::uint32_t graph::vertex_count_get(void) const
{
	return vertices.size();
}

const std::vector<edge> graph::edge_get(const vertex& vertex) const
{
	std::vector<edge> edges_adjacent;

	auto iter = edges_map.find(vertex.id());
	if(iter != std::end(edges_map))
	{
		for(const auto& kvp : edges_map.at(vertex.id()))
		{
			for(const auto& edge : kvp.second)
			{
				edges_adjacent.push_back(*edge);
				continue;
			}
		}
	}

	return edges_adjacent;
}

void graph::edge_add(const edge& edge_new)
{
	const std::uint32_t source_id = edge_new.source_id();
	const std::uint32_t target_id = edge_new.target_id();
	const bool directed = edge_new.directed();

	// create source vertex if not found
	bool vertex_found = vertices.count(source_id) != 0;
	if(!vertex_found)
	{
		vertices[source_id] = new vertex(source_id);
	}

	vertex_found = vertices.count(target_id) != 0;
	if(!vertex_found)
	{
		vertices[target_id] = new vertex(target_id);
	}

	edge* new_edge = new edge(edge_new);
	edges.push_back(new_edge);

	if(directed)
	{
		// add the edge only in one direction
		((edges_map[source_id])[target_id]).push_back(new_edge);
	}
	else
	{
		// add the same edge in both direction
		((edges_map[source_id])[target_id]).push_back(new_edge);
		((edges_map[target_id])[source_id]).push_back(new_edge);
	}
}



}
