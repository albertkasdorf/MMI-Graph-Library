#include <graph.h>

#include <graph_vertex.h>
#include <graph_edge.h>
#include <comparer.h>
#include <vector>

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

void graph::add(const vertex& vertex_new)
{
	bool vertex_found = vertices.count(vertex_new.id()) != 0;
	if(!vertex_found)
	{
		vertices[vertex_new.id()] = new vertex(vertex_new);
	}
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

std::vector<edge> graph::edge_get(void) const
{
	std::vector<edge> edge_all;
	for(const auto& edge : edges)
	{
		edge_all.push_back(*edge);
	}
	return edge_all;
}

void graph::add(const edge& edge_new)
{
	const std::uint32_t source_id = edge_new.source_id();
	const std::uint32_t target_id = edge_new.target_id();
	const bool directed = edge_new.directed();

	// create source vertex if not found
	this->add(vertex(source_id));
	this->add(vertex(target_id));

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

vertex_iterator graph::vertices_begin() const
{
	return vertex_iterator(vertices.begin());
}

vertex_iterator graph::vertices_end() const
{
	return vertex_iterator(vertices.end());
}

void graph::remove(const edge& edge_remove)
{
	std::vector<edge> edge_vector;
	edge* edge_delete = nullptr;

	edge_vector.push_back(edge_remove);
	if(!edge_remove.directed())
	{
		edge_vector.push_back(edge_remove.reverse_direction());
	}

	for(const edge& edge : edge_vector)
	{
		const std::uint32_t source_id = edge.source_id();
		const std::uint32_t target_id = edge.target_id();

		// exist source_id?
		if(edges_map.count(source_id) != 0)
		{
			// exist target_id?
			if(edges_map[source_id].count(target_id) != 0)
			{
				// delete the first equal element of the vector
				for(std::size_t i = 0;
					i < ((edges_map[source_id])[target_id]).size();
					++i)
				{
					if(*((edges_map[source_id])[target_id])[i] != edge_remove)
						continue;

					edge_delete = ((edges_map[source_id])[target_id])[i];
					((edges_map[source_id])[target_id]).erase(
							((edges_map[source_id])[target_id]).begin() + i);
					break;
				}

				// if vector is empty, than remove the target_id from the map
				if(((edges_map[source_id])[target_id]).size() == 0)
				{
					(edges_map[source_id]).erase(target_id);
				}
			}

			// if the target_id map is empty, than remove the source_id from the map
			if(edges_map[source_id].size() == 0)
			{
				edges_map.erase(source_id);
			}
		}
	}

	for(std::vector<edge*>::const_iterator i = edges.begin();
		i != edges.end();
		++i)
	{
		if( *i != edge_delete )
			continue;

		delete *i;
		edges.erase(i);
		break;
	}
}



}
