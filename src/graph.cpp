#include <graph.h>
#include <vector>
#include <graph_vertex.h>
#include <graph_edge.h>


namespace graph
{

graph::graph()
{
}

graph::~graph()
{
}

void graph::add_vertex(const uint32_t id)
{
	const std::size_t hash = vertex::create_hash(id);
	const bool vertex_not_found = vertices.count(hash) == 0;
	if(vertex_not_found)
	{
		vertices[hash] = std::make_shared<vertex>(id);
	}
}

const vertex* graph::get_vertex(const std::uint32_t id) const
{
	return get_vertex_internal(id);
}

vertex* graph::get_vertex_internal(const std::uint32_t id) const
{
	const std::size_t hash = vertex::create_hash(id);
	auto iter = vertices.find(hash);
	return (iter->second).get();
}

std::uint32_t graph::get_vertex_count(void) const
{
	return vertices.size();
}

//const std::vector<edge> graph::edge_get(const vertex& vertex) const
//{
//	std::vector<edge> edges_adjacent;

//	auto iter = edges_map.find(vertex.id());
//	if(iter != std::end(edges_map))
//	{
//		for(const auto& kvp : edges_map.at(vertex.id()))
//		{
//			for(const auto& edge : kvp.second)
//			{
//				edges_adjacent.push_back(*edge);
//				continue;
//			}
//		}
//	}

//	return edges_adjacent;
//}

//std::vector<edge> graph::edge_get(void) const
//{
//	std::vector<edge> edge_all;
//	for(const auto& edge : edges)
//	{
//		edge_all.push_back(*edge);
//	}
//	return edge_all;
//}

void graph::add_edge(const edge* new_edge)
{
	// TODO: Check TWIN condition
	if(new_edge->has_weight())
	{
		add_undirected_edge(
			new_edge->get_source()->get_id(),
			new_edge->get_target()->get_id(),
			new_edge->get_weight());
	}
	else
	{
		add_undirected_edge(
			new_edge->get_source()->get_id(), new_edge->get_target()->get_id());
	}
}

//void graph::add_edge(const edge& new_edge)
//{
//	// TODO: Check TWIN condition

//	if(new_edge.has_weight())
//	{
//		add_undirected_edge(
//			new_edge.get_source()->get_id(), new_edge.get_target()->get_id(), new_edge.get_weight());
//	}
//	else
//	{
//		add_undirected_edge( new_edge.get_source()->get_id(), new_edge.get_target()->get_id());
//	}
//}

void graph::add_undirected_edge(
	const uint32_t& source_id, const uint32_t& target_id)
{
	add_undirected_edge(source_id, target_id, nullptr);
}

void graph::add_undirected_edge(
	const uint32_t& source_id, const uint32_t& target_id, const float& weight)
{
	add_undirected_edge(source_id, target_id, &weight);
}

void graph::add_undirected_edge(
	const uint32_t& source_id, const uint32_t& target_id, const float* weight)
{
	// create source vertex if not found
	add_vertex(source_id);
	add_vertex(target_id);

	vertex* source = get_vertex_internal(source_id);
	vertex* target = get_vertex_internal(target_id);

	std::shared_ptr<edge> src_edge = std::make_shared<edge>();
	std::shared_ptr<edge> tgt_edge = std::make_shared<edge>();

	src_edge->set_source(source);
	tgt_edge->set_source(target);

	src_edge->set_target(target);
	tgt_edge->set_target(source);

	src_edge->set_twin(tgt_edge.get());
	tgt_edge->set_twin(src_edge.get());

	if(weight != nullptr)
	{
		src_edge->set_weight(*weight);
		tgt_edge->set_weight(*weight);
	}

	source->add_edge(src_edge.get());
	target->add_edge(tgt_edge.get());

	const std::size_t hash_src_edge = src_edge->get_hash();
	const std::size_t hash_tgt_edge = tgt_edge->get_hash();

	edges.insert(std::make_pair(hash_src_edge, src_edge));
	edges.insert(std::make_pair(hash_tgt_edge, tgt_edge));
}


std::pair<vertex_iterator, vertex_iterator> graph::get_vertices(void) const
{
	return std::make_pair(
		vertex_iterator(vertices.cbegin()),
		vertex_iterator(vertices.end()));
}

void graph::remove_edge(const edge& edge_remove)
{
//	std::vector<edge> edge_vector;
//	edge* edge_delete = nullptr;

//	edge_vector.push_back(edge_remove);
//	if(!edge_remove.directed())
//	{
//		edge_vector.push_back(edge_remove.reverse_direction());
//	}

//	for(const edge& edge : edge_vector)
//	{
//		const std::uint32_t source_id = edge.source_id();
//		const std::uint32_t target_id = edge.target_id();

//		// exist source_id?
//		if(edges_map.count(source_id) != 0)
//		{
//			// exist target_id?
//			if(edges_map[source_id].count(target_id) != 0)
//			{
//				// delete the first equal element of the vector
//				for(std::size_t i = 0;
//					i < ((edges_map[source_id])[target_id]).size();
//					++i)
//				{
//					if(*((edges_map[source_id])[target_id])[i] != edge_remove)
//						continue;

//					edge_delete = ((edges_map[source_id])[target_id])[i];
//					((edges_map[source_id])[target_id]).erase(
//							((edges_map[source_id])[target_id]).begin() + i);
//					break;
//				}

//				// if vector is empty, than remove the target_id from the map
//				if(((edges_map[source_id])[target_id]).size() == 0)
//				{
//					(edges_map[source_id]).erase(target_id);
//				}
//			}

//			// if the target_id map is empty, than remove the source_id from the map
//			if(edges_map[source_id].size() == 0)
//			{
//				edges_map.erase(source_id);
//			}
//		}
//	}

//	for(std::vector<edge*>::const_iterator i = edges.begin();
//		i != edges.end();
//		++i)
//	{
//		if( *i != edge_delete )
//			continue;

//		delete *i;
//		edges.erase(i);
//		break;
//	}
}

std::pair<edge_iterator_on_multimap, edge_iterator_on_multimap> graph::get_edges(void) const
{
	return std::make_pair(
		edge_iterator_on_multimap(edges.cbegin()),
		edge_iterator_on_multimap(edges.cend()));
}

std::uint32_t graph::get_edge_count(void) const
{
	return edges.size();
}



}
