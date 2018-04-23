#include <graph_algorithm.h>

#include <deque>
#include <memory>
#include <queue>
#include <stack>
#include <cassert>
#include <chrono>
#include <iostream>

#include <graph_vertex.h>
#include <graph.h>
#include <graph_comparer.h>
#include <graph_edge.h>

namespace graph
{

algorithm::algorithm()
{
}

algorithm::~algorithm()
{
}

void algorithm::breadth_first_search(
	const graph* graph_full,
	const vertex* start_vertex,
	graph* graph_sub)
{
	std::deque<const vertex*> processing_queue;
	std::set<const vertex*, compare_vertex_id> vertex_lookup;

	processing_queue.push_back(start_vertex);
	vertex_lookup.insert(start_vertex);

	while(!processing_queue.empty())
	{
		const vertex* vertex_current = processing_queue.front();
		processing_queue.pop_front();

		for(auto current_edge : vertex_current->get_edges())
		{
			const vertex* target_vertex = current_edge->get_target();

			const bool vertex_processed = vertex_lookup.count(target_vertex) != 0;
			if(vertex_processed)
				continue;

			processing_queue.push_back(target_vertex);
			vertex_lookup.insert(target_vertex);

			graph_sub->add_edge(current_edge);

			continue;
		}
	}
	return;
}

void algorithm::depth_first_search(
	const graph* graph_full,
	const vertex* vertex_start,
	graph* graph_sub)
{
	std::set<const vertex*, compare_vertex_id> lookup;

	depth_first_search_recursive(graph_full, vertex_start, &lookup, graph_sub);
}

void algorithm::depth_first_search_recursive(
	const graph* graph_full,
	const vertex* vertex_to_expand,
	std::set<const vertex*, compare_vertex_id>* lookup,
	graph* graph_sub )
{
	lookup->insert(vertex_to_expand);

	for(auto current_edge : vertex_to_expand->get_edges())
	{
		const vertex* next_vertex = current_edge->get_target();
		const bool next_vertex_found = lookup->count(next_vertex);

		if(next_vertex_found)
			continue;

		graph_sub->add_edge(current_edge);
		depth_first_search_recursive(graph_full, next_vertex, lookup, graph_sub);
	}
}

void algorithm::connected_component_with_bfs(
	const graph* graph_full,
	std::vector<std::shared_ptr<graph>>* subgraphs)
{
	connected_component(
		graph_full,
		subgraphs,
		std::bind(&algorithm::breadth_first_search,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
}

void algorithm::connected_component_with_dfs(
	const graph* graph_full,
	std::vector<std::shared_ptr<graph>>* subgraphs)
{
	connected_component(
		graph_full,
		subgraphs,
		std::bind(&algorithm::depth_first_search,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
}

void algorithm::connected_component(
	const graph* graph_full,
	std::vector<std::shared_ptr<graph>>* subgraphs,
	const std::function<void(const graph*, const vertex*, graph*)>& search_algorithm )
{
	std::set<const vertex*, compare_vertex_id> lookup;

	for(auto vertex_current : graph_full->get_vertices())
	{
		const bool vertex_found = lookup.count(vertex_current) != 0;

		if(vertex_found)
			continue;

		std::shared_ptr<graph> graph_sub = std::make_shared<graph>();

		search_algorithm(graph_full, vertex_current, graph_sub.get());

		for(auto vertex_from_sub_graph : graph_sub->get_vertices())
		{
			lookup.insert(vertex_from_sub_graph);
		}

		subgraphs->push_back(graph_sub);
		continue;
	}
}

//
// Find the minimal spanning tree with the prim algorithm.
//
void algorithm::prim(
	const graph* full_graph, const vertex* start_vertex, graph* mst_graph, double* mst_cost)
{
	std::priority_queue<const edge*, std::vector<const edge*>, compare_edge_weight> queue;
	std::set<const vertex*, compare_vertex_id> vertex_lookup;
	*mst_cost = 0.0;

	for(auto edge : start_vertex->get_edges())
	{
		queue.push(edge);
	}
	vertex_lookup.insert(start_vertex);

	while(!queue.empty())
	{
		const edge* new_edge = queue.top();
		queue.pop();

		const vertex* source_vertex = new_edge->get_source();
		const vertex* target_vertex = new_edge->get_target();

		const bool source_found = vertex_lookup.count(source_vertex) != 0;
		const bool target_found = vertex_lookup.count(target_vertex) != 0;

		assert(source_found || target_found);

		if(source_found && target_found)
		{
			// Discard the edge, if both vertices processed.
			continue;
		}

		const vertex* add_vertex = source_found ? target_vertex : source_vertex;

		// Add all edges to the queue from the "unprocessed" vertex.
		vertex_lookup.insert(add_vertex);
		for(auto edge : add_vertex->get_edges())
		{
			queue.push(edge);
		}

		mst_graph->add_edge(new_edge);
		*mst_cost += new_edge->get_weight();
	}
}

//
// Find the minimal spanning tree with the kruskal algorithm.
//
void algorithm::kruskal(const graph* full_graph, graph* mst_graph, double* mst_cost)
{
	std::priority_queue<const edge*, std::vector<const edge*>, compare_edge_weight> queue;
	// Quick lookup of the vertex component id.
	std::map<const vertex*, std::size_t> vertex_lookup;
	// Components groups of vertices
	std::map<std::size_t, std::vector<const vertex*>> component_lookup;
	std::size_t component_id = 0;
	*mst_cost = 0.0;

	for(auto edge : full_graph->get_edges())
	{
		queue.push(edge);
	}

	while(!queue.empty())
	{
		const edge* new_edge = queue.top();
		queue.pop();

		const vertex* source_vertex = new_edge->get_source();
		const vertex* target_vertex = new_edge->get_target();

		auto iter_source = vertex_lookup.find(source_vertex);
		auto iter_target = vertex_lookup.find(target_vertex);

		const bool source_found = iter_source != vertex_lookup.end();
		const bool target_found = iter_target != vertex_lookup.end();

		if(!source_found && !target_found)
		{
			//
			// Add a standalone edge
			//
			vertex_lookup.insert(std::make_pair(source_vertex, component_id));
			vertex_lookup.insert(std::make_pair(target_vertex, component_id));

			component_lookup[component_id].push_back(source_vertex);
			component_lookup[component_id].push_back(target_vertex);

			mst_graph->add_edge(new_edge);
			*mst_cost += new_edge->get_weight();

			++component_id;
		}
		else if(source_found != target_found)
		{
			//
			// Append a edge to a subgraph
			//
			const std::size_t add_component_id =
				source_found ? (*iter_source).second : (*iter_target).second;
			const vertex* add_vertex =
				source_found ? target_vertex : source_vertex;

			vertex_lookup.insert(std::make_pair(add_vertex, add_component_id));
			component_lookup[add_component_id].push_back(add_vertex);

			mst_graph->add_edge(new_edge);
			*mst_cost += new_edge->get_weight();
		}
		else // if(source_found && target_found)
		{
			//
			// Merge two subgraphs if they have a different component id.
			//
			const std::size_t source_component_id = (*iter_source).second;
			const std::size_t target_component_id = (*iter_target).second;

			// Cycle test
			if(source_component_id == target_component_id)
				continue;

			const std::size_t source_count =
				component_lookup[source_component_id].size();
			const std::size_t target_count =
				component_lookup[target_component_id].size();

			const std::size_t from_component_id =
					(source_count < target_count) ? source_component_id : target_component_id;
			const std::size_t to_component_id =
					(source_count < target_count) ? target_component_id : source_component_id;

			// Override the old component_id in vertex_loop
			// Add the vertices from the old component id group to the new one.
			for(auto vertex : component_lookup[from_component_id])
			{
				vertex_lookup[vertex] = to_component_id;
				component_lookup[to_component_id].push_back(vertex);
			}
			component_lookup.erase(from_component_id);

			mst_graph->add_edge(new_edge);
			*mst_cost += new_edge->get_weight();
		}
	}
	assert(component_lookup.size() == 1);
}

}
