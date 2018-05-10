#include <graph_algorithm.h>

#include <deque>
#include <memory>
#include <queue>
#include <stack>
#include <cassert>
#include <chrono>
#include <iostream>
#include <unordered_set>

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

void algorithm::nearest_neighbor(
	const graph* full_graph, const vertex* start_vertex, graph* hamilton_graph)
{
	const std::size_t vertex_count = full_graph->get_vertex_count();
	const vertex* current_vertex = start_vertex;
	const edge* next_edge = nullptr;
	std::set<const vertex*, compare_vertex_id> vertex_lookup;

	// Visited every vertex in the graph
	for(std::size_t i = 0; i < vertex_count; ++i)
	{
		// we have visited this vertex
		vertex_lookup.insert(current_vertex);

		// if this is the last vertex,
		// remove the start_vertex from lookup to get only this edge.
		if(i == (vertex_count - 1))
		{
			vertex_lookup.erase(start_vertex);
		}

		for(auto edge : current_vertex->get_edges())
		{
			assert(edge->has_weight());
			assert(edge->get_source()->get_id() == current_vertex->get_id());

			// Do the edge point to an visited vertex?
			const bool vertex_visited = vertex_lookup.count(edge->get_target()) != 0;
			if(vertex_visited)
			{
				continue;
			}

			// is this edge the first edge that point to an unvisited vertex?
			if(next_edge == nullptr)
			{
				next_edge = edge;
				continue;
			}

			// Do we have an edge that is better than the current best edge?
			if(next_edge->get_weight() > edge->get_weight())
			{
				next_edge = edge;
			}
		}

		hamilton_graph->add_edge(next_edge);
		current_vertex = next_edge->get_target();
		next_edge = nullptr;
	}

	// Do I get back to the starting point?
	assert(start_vertex->get_id() == current_vertex->get_id());
}

void algorithm::double_tree(
	const graph* full_graph, const vertex* start_vertex, graph* hamilton_graph)
{
	graph mst_graph;
	double cost_mst = 0.0;
	std::set<const vertex*, compare_vertex_id> vertex_lookup;
	std::stack<const vertex*> dfs_stack;
	const vertex* current_vertex = nullptr;
	const vertex* previous_vertex = nullptr;
	const edge* connecting_edge = nullptr;

	// get the minimal spanning tree with kruskal
	kruskal(full_graph, &mst_graph, &cost_mst);

	// get the start vertex from mst_graph an put them into the stack
	current_vertex = mst_graph.get_vertex(start_vertex->get_id());

	dfs_stack.push(current_vertex);
	vertex_lookup.insert(current_vertex);

	while(!dfs_stack.empty())
	{
		current_vertex = dfs_stack.top();
		dfs_stack.pop();

		// add all vertices, that are not already on the stack
		for(auto edge : current_vertex->get_edges())
		{
			assert(edge->has_weight());
			assert(edge->get_source()->get_id() == current_vertex->get_id());

			// Do the edge point to an vertex at is already on the stack?
			const bool vertex_on_stack = vertex_lookup.count(edge->get_target()) != 0;
			if(vertex_on_stack)
			{
				continue;
			}

			dfs_stack.push(edge->get_target());
			vertex_lookup.insert(edge->get_target());
		}

		// previous_vertex == nullptr means that this is the first run and
		// we do not have two vertices
		if(previous_vertex != nullptr)
		{
			// Search for the connecting edge in the full_graph
			connecting_edge =
				full_graph->get_edge(previous_vertex, current_vertex);
			hamilton_graph->add_edge(connecting_edge);
		}

		previous_vertex = current_vertex;
	}

	// At least search for the connecting edge in the full_graph between
	// the previous_vertex (last vertex) and the start_vertex
	connecting_edge = full_graph->get_edge(previous_vertex, start_vertex);
	hamilton_graph->add_edge(connecting_edge);

	return;
}

void algorithm::try_all_routes(
	const graph* complete_graph,
	const vertex* start_vertex,
	const bool use_branch_and_bound,
	graph* hamilton_graph)
{
	std::size_t vertex_count = complete_graph->get_vertex_count();
	std::stack<const edge*> current_tour;
	std::set<const vertex*> visited_vertices;
	std::stack<const edge*> best_tour;
	double best_cost = std::numeric_limits<double>::infinity();

	try_all_routes_recursive(
		complete_graph->get_vertex(start_vertex->get_id()),
		&use_branch_and_bound,
		&vertex_count,
		complete_graph->get_vertex(start_vertex->get_id()),
		0.0,
		&current_tour,
		&visited_vertices,
		&best_tour,
		&best_cost );

	while(!best_tour.empty())
	{
		hamilton_graph->add_edge(best_tour.top());
		best_tour.pop();
	}

	return;
}

void algorithm::try_all_routes_recursive(
	const vertex* const start_vertex,					// From this we started
	const bool* const use_branch_and_bound,				// do not go deeper if the cost worse than the best_cost
	const std::size_t* const number_of_vertices,
	const vertex* current_vertex,
	const double current_cost,							// cost the get to the current vertex
	std::stack<const edge*>* const current_tour,		// list of edges to the get to the current vertex
	std::set<const vertex*>* const visited_vertices,
	std::stack<const edge*>* const best_tour,
	double* const best_cost)
{
	const edge* edge_to_start_vertex = nullptr;

	visited_vertices->insert(current_vertex);


	for(auto edge : current_vertex->get_edges())
	{
		const vertex* next_vertex = edge->get_target();
		const double new_cost = current_cost + edge->get_weight();

		if( next_vertex->get_id() == start_vertex->get_id() )
			edge_to_start_vertex = edge;

		if(use_branch_and_bound && (new_cost >= *best_cost))
			continue;

		const bool vertex_visited = visited_vertices->count(next_vertex) != 0;
		if(vertex_visited)
			continue;

		current_tour->push(edge);
		{
			try_all_routes_recursive(
				start_vertex,
				use_branch_and_bound,
				number_of_vertices,
				next_vertex,
				new_cost,
				current_tour,
				visited_vertices,
				best_tour,
				best_cost );
		}
		current_tour->pop();
	}

	if(visited_vertices->size() == *number_of_vertices)
	{
		assert(edge_to_start_vertex != nullptr);

		const double final_cost = current_cost + edge_to_start_vertex->get_weight();
		if(final_cost < *best_cost)
		{
			current_tour->push(edge_to_start_vertex);
			*best_tour = *current_tour;
			*best_cost = final_cost;
			current_tour->pop();
		}
	}

	// we move down to the bottom of the call stack, so remove the current_vertex
	visited_vertices->erase(current_vertex);
}
















}
