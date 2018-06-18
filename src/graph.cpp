#include <graph.h>
#include <vector>
#include <cassert>
#include <graph_vertex.h>
#include <graph_edge.h>


namespace graph
{

graph::graph()
{
}

graph::graph(const graph& rhs)
{
	for(auto v : rhs.get_vertices())
	{
		const std::size_t hash_of_v = vertex::create_hash(v->get_id());
		std::shared_ptr<vertex> copy_of_v = std::make_shared<vertex>(v->get_id());

		if(v->has_balance())
			copy_of_v->set_balance(v->get_balance());

		vertices[hash_of_v] = copy_of_v;
	}

	// TODO: Twin handling
	for(auto e : rhs.get_edges())
	{
		std::shared_ptr<edge> copy_of_e = std::make_shared<edge>();

		if(e->has_weight())
			copy_of_e->set_weight(e->get_weight());

		if(e->has_cost())
			copy_of_e->set_cost(e->get_cost());

		if(e->has_capacity())
			copy_of_e->set_capacity(e->get_capacity());

		vertex* v_source = get_vertex_internal(e->get_source()->get_id());
		vertex* v_target = get_vertex_internal(e->get_target()->get_id());

		copy_of_e->set_source(v_source);
		copy_of_e->set_target(v_target);

		v_source->add_edge(copy_of_e.get());

		const std::size_t hash = copy_of_e->get_hash();
		edges.insert(std::make_pair(hash, copy_of_e));
	}
}

graph::~graph()
{
}

const vertex* graph::add_vertex(const uint32_t id)
{
	const std::size_t hash = vertex::create_hash(id);
	const bool vertex_not_found = vertices.count(hash) == 0;

	if(vertex_not_found)
	{
		vertices[hash] = std::make_shared<vertex>(id);
	}

	return vertices[hash].get();
}

void graph::add_vertex(const uint32_t id, const double balance)
{
	const std::size_t hash = vertex::create_hash(id);
	const bool vertex_not_found = vertices.count(hash) == 0;
	if(vertex_not_found)
	{
		auto v = std::make_shared<vertex>(id);
		v->set_balance(balance);

		vertices[hash] = v;
	}
}

const vertex* graph::add_vertex(const uint32_t* id, const double* balance)
{
	std::shared_ptr<vertex> v;

	if(id == nullptr)
	{
		std::uint32_t max_id = 0;
		for(auto item : vertices)
			max_id = std::max(max_id, item.second->get_id());
		v = std::make_shared<vertex>(++max_id);
	}
	else
	{
		v = std::make_shared<vertex>(*id);
	}

	if(balance != nullptr)
		v->set_balance(*balance);

	const std::size_t hash = vertex::create_hash(v->get_id());
	assert(vertices.count(hash) == 0);

	vertices[hash] = v;

	return v.get();
}

const vertex* graph::add_vertex(const vertex* v)
{
	std::shared_ptr<vertex> copy = v->create_copy();
	const std::size_t hash = vertex::create_hash(*copy);

	if(vertices.count(hash) == 0)
	{
		vertices[hash] = copy;
	}

	return vertices[hash].get();
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
	const uint32_t source_id = new_edge->get_source()->get_id();
	const uint32_t target_id = new_edge->get_target()->get_id();

	add_vertex(source_id);
	add_vertex(target_id);

	vertex* source = get_vertex_internal(source_id);
	vertex* target = get_vertex_internal(target_id);

	std::shared_ptr<edge> forward_edge = new_edge->create_copy();

	forward_edge->set_source(source);
	forward_edge->set_target(target);

	source->add_edge(forward_edge.get());

	const std::size_t forward_edge_hash = forward_edge->get_hash();
	edges.insert(std::make_pair(forward_edge_hash, forward_edge));

	if(new_edge->has_twin())
	{
		std::shared_ptr<edge> backward_edge = new_edge->create_copy();

		backward_edge->set_source(target);
		backward_edge->set_target(source);

		target->add_edge(backward_edge.get());

		forward_edge->set_twin(backward_edge.get());
		backward_edge->set_twin(forward_edge.get());

		const std::size_t backward_edge_hash = backward_edge->get_hash();
		edges.insert(std::make_pair(backward_edge_hash, backward_edge));
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
	const uint32_t& source_id, const uint32_t& target_id, const double& weight)
{
	add_undirected_edge(source_id, target_id, &weight);
}

void graph::add_undirected_edge(
	const uint32_t& source_id, const uint32_t& target_id, const double* weight)
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

void graph::add_directed_edge(
	const uint32_t& source_id, const uint32_t& target_id, const double& weight)
{
	add_directed_edge(source_id, target_id, &weight);
}

void graph::add_directed_edge(
	const uint32_t& source_id, const uint32_t& target_id, const double* weight)
{
	// create source vertex if not found
	add_vertex(source_id);
	add_vertex(target_id);

	vertex* source = get_vertex_internal(source_id);
	vertex* target = get_vertex_internal(target_id);

	std::shared_ptr<edge> src_tgt_edge = std::make_shared<edge>();

	src_tgt_edge->set_source(source);
	src_tgt_edge->set_target(target);

	if(weight != nullptr)
	{
		src_tgt_edge->set_weight(*weight);
	}

	source->add_edge(src_tgt_edge.get());

	const std::size_t hash_src_edge = src_tgt_edge->get_hash();

	edges.insert(std::make_pair(hash_src_edge, src_tgt_edge));
}

void graph::add_directed_edge(
	const uint32_t source_id,
	const uint32_t target_id,
	const double cost,
	const double capacity)
{
	// create source vertex if not found
	add_vertex(source_id);
	add_vertex(target_id);

	vertex* source = get_vertex_internal(source_id);
	vertex* target = get_vertex_internal(target_id);

	auto src_tgt_edge = std::make_shared<edge>();

	src_tgt_edge->set_source(source);
	src_tgt_edge->set_target(target);
	src_tgt_edge->set_cost(cost);
	src_tgt_edge->set_capacity(capacity);

	source->add_edge(src_tgt_edge.get());

	const std::size_t hash_src_edge = src_tgt_edge->get_hash();

	edges.insert(std::make_pair(hash_src_edge, src_tgt_edge));
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

std::uint32_t graph::get_edge_count(void) const
{
	return edges.size();
}

const edge* graph::get_edge(
	const vertex* source_vertex, const vertex* target_vertex) const
{
	const edge* result_edge = nullptr;
	const vertex* search_vertex = get_vertex(source_vertex->get_id());

	for(auto edge : search_vertex->get_edges())
	{
		// Edge should "point" in the right direction
		assert(edge->get_source()->get_id() == search_vertex->get_id());

		if(target_vertex->get_id() == edge->get_target()->get_id())
		{
			// More than one edge found.
			assert(result_edge ==  nullptr);

			result_edge = edge;
		}
	}

	// No edge found
	assert(result_edge);

	return result_edge;
}

const edge* graph::get_edge(const edge* e) const
{
	const edge* result_edge = nullptr;

	for(const edge* e2 : get_edges())
	{
		const bool source_equal = e2->get_source()->get_id() == e->get_source()->get_id();
		const bool target_equal = e2->get_target()->get_id() == e->get_target()->get_id();

		if(source_equal && target_equal)
		{
			result_edge = e2;
			break;
		}
	}

	assert(result_edge != nullptr);
	return result_edge;
}



}
