#include <graph_algorithm.h>

#include <deque>
#include <memory>
#include <queue>
#include <stack>
#include <cassert>
#include <chrono>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <string>
#include <list>

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

	// Visit every vertex in the graph
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

			// Do the edge point to an vertex that is already on the stack?
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
}

//
// Iterativer Ansatz
//
//void algorithm::try_all_routes(
//	const graph* complete_graph,
//	const vertex* start_vertex,
//	const bool use_branch_and_bound,
//	graph* hamilton_graph)
//{
//	std::stack<const vertex*> vertex_stack;
//	std::stack<const vertex*> tour_stack;
//	std::stack<std::size_t> level_stack;
//	std::set<const vertex*> visited_vertices;
//	std::size_t last_level = 0;

//	vertex_stack.push(complete_graph->get_vertex(start_vertex->get_id()));
//	level_stack.push(last_level + 1);

//	while(!vertex_stack.empty())
//	{
//		const vertex* current_vertex = vertex_stack.top();
//		vertex_stack.pop();

//		const std::size_t current_level = level_stack.top();
//		level_stack.pop();

//		if(last_level < current_level)
//		{
//		}
//		else if(last_level >= current_level)
//		{
//			for(std::size_t i = 0; i <= (last_level - current_level); ++i )
//			{
//				visited_vertices.erase(tour_stack.top());
//				tour_stack.pop();
//			}
//		}
//		tour_stack.push(current_vertex);
//		visited_vertices.insert(current_vertex);

//		for(auto edge : current_vertex->get_edges())
//		{
//			const vertex* next_vertex = edge->get_target();

//			const bool vertex_visited = visited_vertices.count(next_vertex) != 0;
//			if(vertex_visited)
//				continue;


//			vertex_stack.push(next_vertex);
//			level_stack.push(current_level + 1);
//		}

//		last_level = current_level;
//	}
//}

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

		// store the edge to the start point
		if( next_vertex->get_id() == start_vertex->get_id() )
			edge_to_start_vertex = edge;

		// Do not go deeper if the cost worse than the best_cost
		if(use_branch_and_bound && (new_cost >= *best_cost))
			continue;

		// Ignore already visited vertices
		const bool vertex_visited = visited_vertices->count(next_vertex) != 0;
		if(vertex_visited)
			continue;

		// We go deeper, so add the edge and remove when we go up
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

	// We had visited all vertices, so compute the final_cost to the start_vertex
	// and update the best_tour/best_cost if the final_cost are cheaper.
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

	// we reached the bottom of the call stack, so remove the current_vertex
	visited_vertices->erase(current_vertex);
}

//
// Dijkstra-Algorithm
//
void algorithm::dijkstra(
	const graph* full_graph,
	const vertex* start_vertex,
	graph* shortest_path_graph,
	std::map<const vertex*, double>* distances,		// Distances from the start_vertex to the other vertices.
	bool* negative_weights_found)
{
	// Edge from the predecessor vertex. (target == vertex)
	std::map<const vertex*, const edge*> predecessor;
	std::deque<const vertex*> unprocessed_vertices;

	const vertex* current_vertex = nullptr;
	double current_vertex_distance = std::numeric_limits<double>::infinity();

	distances->clear();
	*negative_weights_found = false;

	// Initialize distances, predecessor and unprocessed_vertices
	for(auto vertex : full_graph->get_vertices())
	{
		double initial_distance = std::numeric_limits<double>::infinity();

		if(vertex->get_id() == start_vertex->get_id())
		{
			initial_distance = 0.0;
		}

		distances->insert(
			std::make_pair(vertex, initial_distance));

		predecessor.insert(
			std::make_pair(vertex, nullptr));

		unprocessed_vertices.insert(
			std::end(unprocessed_vertices), vertex);
	}

	// start algorithm with the start_vertex
	current_vertex = full_graph->get_vertex(start_vertex->get_id());
	current_vertex_distance = (*distances)[current_vertex];

	while(true)
	{
		for(auto explore_edge : current_vertex->get_edges())
		{
			assert(current_vertex == explore_edge->get_source());
			assert(explore_edge->has_weight());

			if(explore_edge->get_weight() < 0.0)
			{
				*negative_weights_found = true;
			}

			const vertex* target_vertex = explore_edge->get_target();

			// What is my current distance from start_vertex to target_vertex
			const double target_vertex_distance = (*distances)[target_vertex];

			const double new_distance =
				current_vertex_distance + explore_edge->get_weight();

			// If the new route is better, update the data
			if(new_distance < target_vertex_distance)
			{
				(*distances)[target_vertex] = new_distance;
				predecessor[target_vertex] = explore_edge;
			}
		}

		// Remove examined vertex
		unprocessed_vertices.erase(
			std::remove(
				std::begin(unprocessed_vertices),
				std::end(unprocessed_vertices),
				current_vertex));

		// If all vertices processed, stop the endless loop
		if(unprocessed_vertices.empty())
			break;

		// Find the next vertex with the lowest distance
		current_vertex = nullptr;
		current_vertex_distance = std::numeric_limits<double>::infinity();
		for(auto next_vertex : unprocessed_vertices)
		{
			const double next_vertex_distance = (*distances)[next_vertex];

			if(next_vertex_distance <= current_vertex_distance)
			{
				current_vertex = next_vertex;
				current_vertex_distance = next_vertex_distance;
			}
		}
	}

	// Build shortest path graph
	for(auto kvp : predecessor)
	{
		// The start_vertex has no predecessor
		if(kvp.second == nullptr)
			continue;

		shortest_path_graph->add_directed_edge(
			kvp.second->get_source()->get_id(),
			kvp.second->get_target()->get_id(),
			kvp.second->get_weight());
	}

	return;
}

//
// Moore-Bellman-Ford-Algorithm
//
void algorithm::moore_bellman_ford(
	const graph* full_graph,
	const vertex* start_vertex,
	graph* shortest_path_graph,
	std::map<const vertex*, double>* distances,
	bool* negative_cycle_found)
{
	// Edge from the predecessor vertex. (target == vertex)
	std::map<const vertex*, const edge*> predecessor;


	// Initialize distances and predecessor
	for(auto vertex : full_graph->get_vertices())
	{
		double initial_distance = std::numeric_limits<double>::infinity();

		if(vertex->get_id() == start_vertex->get_id())
		{
			initial_distance = 0.0;
		}

		distances->insert(
			std::make_pair(vertex, initial_distance));

		predecessor.insert(
			std::make_pair(vertex, nullptr));
	}

	// Compute the distances and the predecessor
	for(std::uint32_t i = 0; i < full_graph->get_vertex_count() - 1; ++i)
	{
		for(auto edge : full_graph->get_edges())
		{
			const vertex* source_vertex = edge->get_source();
			const vertex* target_vertex = edge->get_target();

			const double source_distance = (*distances)[source_vertex];
			const double target_distance = (*distances)[target_vertex];

			const double new_distance = source_distance + edge->get_weight();

			if(new_distance < target_distance)
			{
				(*distances)[target_vertex] = new_distance;
				predecessor[target_vertex] = edge;
			}
		}
	}

	// Detect the negative cycle.
	for(auto edge : full_graph->get_edges())
	{
		const vertex* source_vertex = edge->get_source();
		const vertex* target_vertex = edge->get_target();

		const double source_distance = (*distances)[source_vertex];
		const double target_distance = (*distances)[target_vertex];

		const double new_distance = source_distance + edge->get_weight();

		if(new_distance < target_distance)
		{
			*negative_cycle_found = true;
			break;
		}
	}

	// Build shortest path graph
	for(auto kvp : predecessor)
	{
		// The start_vertex has no predecessor
		if(kvp.second == nullptr)
			continue;

		shortest_path_graph->add_directed_edge(
			kvp.second->get_source()->get_id(),
			kvp.second->get_target()->get_id(),
			kvp.second->get_weight());
	}

	return;
}

void algorithm::edmonds_karp(
	const graph* full_graph,
	const vertex* source_vertex,
	const vertex* target_vertex,
	double* maximum_flow)
{
	std::function<double(const edge*)> capacity_of_edge = [](const edge* e)
	{
		return e->get_weight();
	};
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> flow_per_edge;

	edmonds_karp(
		full_graph,
		source_vertex,
		target_vertex,
		&flow_per_edge,
		maximum_flow,
		capacity_of_edge);
}

void algorithm::edmonds_karp(
	const graph* full_graph,
	const vertex* source_vertex,
	const vertex* target_vertex,
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	double* maximum_flow,
	std::function<double(const edge*)> capacity_of_edge)
{
	// Schritt 1: Setzen Sie f(e)=0 für alle Kanten e∈E.
	for(auto edge : full_graph->get_edges())
	{
		flow_per_edge->insert(std::make_pair(edge, 0.0));
	}

	while(true)
	{
		graph residual_graph;
		std::list<const edge*> shortest_path;
		double shortest_path_min_value = std::numeric_limits<double>::infinity();


		// Schritt 2: Bestimmen Sie G^f und u^f(e).
		create_residual_graph(full_graph, flow_per_edge, &residual_graph, capacity_of_edge);

		// Schritt 3: Konstruieren Sie einen kürzesten (s,t)-Weg p
		// bzgl. der Anzahl der Kanten in G^f.
		get_shortest_path(
			&residual_graph,
			source_vertex,
			target_vertex,
			&shortest_path,
			&shortest_path_min_value,
			capacity_of_edge);

		// Falls keine Pfad existiert: STOPP.
		if(shortest_path.empty())
			break;

		// Schritt 4: Verändern Sie den Fluss f entlang des Wegs p um
		// gamma := min_e∈p u^f(e).
		// => für jedes Pfad element in f suchen und den wert anpassen
		for(auto edge_on_path : shortest_path)
		{
			auto iter = flow_per_edge->find(edge_on_path);

			const edge* edge_from_flow = iter->first;
			const double edge_value_from_flow = iter->second;

			// Edges pointing in the same direction?
			if(edge_from_flow->get_source()->get_id() == edge_on_path->get_source()->get_id())
			{
				(*flow_per_edge)[edge_on_path] =
					edge_value_from_flow + shortest_path_min_value;
			}
			else
			{
				(*flow_per_edge)[edge_on_path] =
					edge_value_from_flow - shortest_path_min_value;
			}
		}

		// Schritt 5: Gehen Sie zu Schritt 2.
	}

	*maximum_flow = 0.0;
	for(auto edge_from_source : source_vertex->get_edges())
	{
		*maximum_flow += (*flow_per_edge)[edge_from_source];
	}

	return;
}

void algorithm::create_residual_graph(
	const graph* full_graph,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	graph* residual_graph,
	std::function<double(const edge*)> capacity_of_edge)
{
	for(auto v : full_graph->get_vertices())
	{
		std::shared_ptr<vertex> copy = v->create_copy();
		residual_graph->add_vertex(copy.get());
	}

	for(auto e : full_graph->get_edges())
	{
		const std::uint32_t source_id = e->get_source()->get_id();
		const std::uint32_t target_id = e->get_target()->get_id();

		// u(e)
		const double edge_capacity = capacity_of_edge(e);
		// f(e)
		const double edge_value = flow_per_edge->at(e);

		// residual_capacity
		// Forward: u^f(e) = u(e) - f(e)
		const double uf_forward_edge = edge_capacity - edge_value;
		// Backward: u^f(e) = f(e)
		const double uf_backward_edge = edge_value;

		if( uf_forward_edge > 0.0 )
		{
			std::shared_ptr<edge> forward_edge = e->create_copy();

			forward_edge->set_capacity(uf_forward_edge);
			forward_edge->set_source(residual_graph->get_vertex(source_id));
			forward_edge->set_target(residual_graph->get_vertex(target_id));

			// Insert forward edge
			residual_graph->add_edge(forward_edge.get());
		}

		if(uf_backward_edge > 0.0)
		{
			std::shared_ptr<edge> backward_edge = e->create_copy();

			backward_edge->set_capacity(uf_backward_edge);
			backward_edge->set_source(residual_graph->get_vertex(target_id));
			backward_edge->set_target(residual_graph->get_vertex(source_id));

			// Insert backward edge
			residual_graph->add_edge(backward_edge.get());
		}
	}
}

void algorithm::get_shortest_path(
	const graph* residual_graph,
	const vertex* source_vertex,
	const vertex* target_vertex,
	std::list<const edge*>* shortest_path,
	double* shortest_path_min_value,
	std::function<double(const edge*)> capacity_of_edge)
{
	std::deque<const vertex*> frontier;
	std::set<const vertex*, compare_vertex_id> lookup;
	std::map<const vertex*, const edge*> predecessor;


	source_vertex = residual_graph->get_vertex(source_vertex->get_id());
	target_vertex = residual_graph->get_vertex(target_vertex->get_id());

	frontier.push_back(source_vertex);
	lookup.insert(source_vertex);
	predecessor.insert(std::make_pair(source_vertex, nullptr));

	while(!frontier.empty())
	{
		const vertex* vertex_current = frontier.front();
		frontier.pop_front();

		for(auto current_edge : vertex_current->get_edges())
		{
			const vertex* target_vertex_of_edge = current_edge->get_target();

			const bool vertex_processed = lookup.count(target_vertex_of_edge) != 0;
			if(vertex_processed)
				continue;

			frontier.push_back(target_vertex_of_edge);
			lookup.insert(target_vertex_of_edge);
			predecessor.insert(std::make_pair(target_vertex_of_edge, current_edge));

			// Have we found the goal?
			if(target_vertex_of_edge->get_id() == target_vertex->get_id())
			{
				frontier.clear();
				break;
			}

			continue;
		}
	}

	const edge* edge_to_predecessor = nullptr;
	for(const vertex* iter = target_vertex;
		(edge_to_predecessor = predecessor[iter]);
		iter = edge_to_predecessor->get_source())
	{
		// residual_capacity
		const double edge_capacity = capacity_of_edge(edge_to_predecessor);

		shortest_path->push_front(edge_to_predecessor);

		*shortest_path_min_value = std::min(
			*shortest_path_min_value, edge_capacity);
	}
}

void algorithm::cycle_cancelling(
	const graph* full_graph,
	bool* minimum_cost_flow_found,
	double* minimum_cost_flow)
{
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> flow_per_edge;
	bool b_flow_exist = false;

	*minimum_cost_flow_found = false;
	*minimum_cost_flow = 0.0;

	// Schritt 1:
	// Berechnung eines b-Flusses. Wenn kein b-Fluss gefunden werden kann,
	// terminiert der Algorithmus ohne Ergebnis.
	// - Add common source and target
	// - Run edmonds_karp between s* and t*
	// - maximum_flow == Sum of b(v) with b(v) > 0
	cc_compute_b_flow(full_graph, &b_flow_exist, &flow_per_edge);

//	std::cout << "Flow: ";
//	for(auto kvp : flow_per_edge)
//	{
//		std::cout << kvp.first->get_source()->get_id() << "->";
//		std::cout << kvp.first->get_target()->get_id() << ", ";
//		std::cout << kvp.second << "; ";
//	}
//	std::cout << std::endl << std::endl;

	if(!b_flow_exist)
		return;

	while(true)
	{
		// Schritt 2:
		// Bestimmung des Residualgraphen, der Residualkapazitäten und der
		// Residualkosten.
		graph residual_graph;
		cc_create_residual_graph(full_graph, &flow_per_edge, &residual_graph);

//		std::cout << "Residual: ";
//		for(const edge* e : residual_graph.get_edges())
//		{
//			std::cout << e->get_source()->get_id() << "->" << e->get_target()->get_id();
//			std::cout << ", " << e->get_capacity() << "/" << e->get_cost() << "; ";
//		}
//		std::cout << std::endl << std::endl;

		// Schritt 3:
		// Konstruieren eines f-augmentierenden Zykels Z in G f mit negativen Kosten.
		// Falls keiner existiert: STOPP
		std::list<const edge*> cycle;
		double gamma = 0.0;
		cc_find_negative_cycle(&residual_graph, &flow_per_edge, &cycle, &gamma);

		if(cycle.empty())
			break;

		// Schritt 4:
		// Verändern des b-Flusses entlang des Zykels um γ.
		for(const edge* e : cycle)
		{
			auto iter = flow_per_edge.find(e);
			assert(iter != flow_per_edge.end());
			const edge* e2 = (*iter).first;

			if(e2->get_source()->get_id() == e->get_source()->get_id())
				flow_per_edge[e] += gamma;
			else
				flow_per_edge[e] -= gamma;
		}

//		std::cout << "Flow: ";
//		for(auto kvp : flow_per_edge)
//		{
//			std::cout << kvp.first->get_source()->get_id() << "->";
//			std::cout << kvp.first->get_target()->get_id() << ", ";
//			std::cout << kvp.second << "; ";
//		}
//		std::cout << std::endl << std::endl;

		// Schritt 5:
		// Ab Schritt 2 wiederholen.
	}

	*minimum_cost_flow_found = true;
	for(auto kvp : flow_per_edge)
	{
		*minimum_cost_flow += kvp.first->get_cost() * kvp.second;
	}

	return;
}

void algorithm::cc_compute_b_flow(
	const graph* full_graph,
	bool* b_flow_exist,
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge)
{
	graph b_flow_graph(*full_graph);
	double sum_source_flow = 0.0;
	double sum_target_flow = 0.0;
	std::vector<const vertex*> src_or_tgt_vert;

	for(auto v : b_flow_graph.get_vertices())
	{
		assert(v->has_balance());
		const double balance = v->get_balance();

		if(balance == 0.0)
			continue;

		if(balance > 0.0)
		{
			sum_source_flow += balance;
		}
		else // if(balance < 0.0)
		{
			sum_target_flow += balance;
		}

		src_or_tgt_vert.push_back(v);
	}

	const vertex* super_source = b_flow_graph.add_vertex(nullptr, &sum_source_flow);
	const vertex* super_target = b_flow_graph.add_vertex(nullptr, &sum_target_flow);

	for(auto v : src_or_tgt_vert)
	{
		const double balance = v->get_balance();
		const double cost = 0.0;
		std::shared_ptr<edge> e = std::make_shared<edge>();

		if(balance > 0.0)
		{
			e->set_source(super_source);
			e->set_target(v);
			e->set_capacity(balance);
			e->set_cost(cost);
		}
		else // if(balance < 0.0)
		{
			e->set_source(v);
			e->set_target(super_target);
			e->set_capacity(balance * -1.0);
			e->set_cost(cost);
		}

		b_flow_graph.add_edge(e.get());
	}

	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> flow_per_edge_internal;
	double maximum_flow = 0.0;
	std::function<double(const edge*)> capacity_of_edge = [](const edge* e)
	{
		return e->get_capacity();
	};

	edmonds_karp(
		&b_flow_graph,
		super_source,
		super_target,
		&flow_per_edge_internal,
		&maximum_flow,
		capacity_of_edge);

	*b_flow_exist = (sum_source_flow == maximum_flow);

	for(const edge* e : full_graph->get_edges())
	{
		const double flow = flow_per_edge_internal[e];
		flow_per_edge->insert(std::make_pair(e, flow));
	}

	return;
}

void algorithm::cc_create_residual_graph(
	const graph* full_graph,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	graph* residual_graph)
{
	for(const vertex* v : full_graph->get_vertices())
	{
		residual_graph->add_vertex(v);
	}

	for(auto e : full_graph->get_edges())
	{
		const std::uint32_t source_id = e->get_source()->get_id();
		const std::uint32_t target_id = e->get_target()->get_id();

		// u(e)
		const double edge_capacity = e->get_capacity();
		// f(e)
		const double edge_value = flow_per_edge->at(e);

		// residual_capacity
		// Forward: u^f(e) = u(e) - f(e)
		const double uf_forward_edge = edge_capacity - edge_value;
		// Backward: u^f(e) = f(e)
		const double uf_backward_edge = edge_value;

		if( uf_forward_edge > 0.0 )
		{
			std::shared_ptr<edge> forward_edge = e->create_copy();

			forward_edge->set_capacity(uf_forward_edge);
			forward_edge->set_cost(e->get_cost());
			forward_edge->set_source(residual_graph->get_vertex(source_id));
			forward_edge->set_target(residual_graph->get_vertex(target_id));

			// Insert forward edge
			residual_graph->add_edge(forward_edge.get());
		}

		if(uf_backward_edge > 0.0)
		{
			std::shared_ptr<edge> backward_edge = e->create_copy();

			backward_edge->set_capacity(uf_backward_edge);
			backward_edge->set_cost(e->get_cost() * -1.0);
			backward_edge->set_source(residual_graph->get_vertex(target_id));
			backward_edge->set_target(residual_graph->get_vertex(source_id));

			// Insert backward edge
			residual_graph->add_edge(backward_edge.get());
		}
	}

	return;
}

void algorithm::cc_find_negative_cycle(
	const graph* residual_graph,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	std::list<const edge*>* cycle,
	double* gamma)
{
	graph search_graph(*residual_graph);

	const vertex* start_vertex = search_graph.add_vertex(nullptr, nullptr);
	for(const vertex* v : search_graph.get_vertices())
	{
		if(v->get_id() == start_vertex->get_id())
			continue;

		edge e;
		e.set_source(start_vertex);
		e.set_target(v);
		e.set_cost(0.0);
		search_graph.add_edge(&e);
	}

	// Edge from the predecessor vertex. (target == vertex)
	std::map<const vertex*, double> distances;
	std::map<const vertex*, const edge*> predecessor;

	// Initialize distances and predecessor
	for(const vertex* v : search_graph.get_vertices())
	{
		double initial_distance = std::numeric_limits<double>::infinity();

		if(v->get_id() == start_vertex->get_id())
		{
			initial_distance = 0.0;
		}

		distances.insert(std::make_pair(v, initial_distance));
		predecessor.insert(std::make_pair(v, nullptr));
	}

	// Compute the distances and the predecessor
	const std::uint32_t vertex_count = search_graph.get_vertex_count();
	std::list<const vertex*> changed_set;

	for(std::uint32_t i = 0; i < vertex_count; ++i)
	{
		const bool is_last_round = ((i + 1) == vertex_count);

		for(auto edge : search_graph.get_edges())
		{
			const vertex* source_vertex = edge->get_source();
			const vertex* target_vertex = edge->get_target();

			const double source_distance = distances[source_vertex];
			const double target_distance = distances[target_vertex];

			const double new_distance = source_distance + edge->get_cost();

			if(new_distance < target_distance)
			{
				distances[target_vertex] = new_distance;
				predecessor[target_vertex] = edge;

				if(is_last_round)
				{
					changed_set.push_back(target_vertex);
				}
			}
		}
	}

	if(!changed_set.empty())
	{
		const vertex* v = changed_set.front();
		std::set<const vertex*, compare_vertex_id> lookup;

		for(std::uint32_t i = 0; i < vertex_count; ++i)
		{
			const edge* e = predecessor[v];
			v = e->get_source();
			continue;
		}

		*gamma = std::numeric_limits<double>::infinity();
		while(lookup.count(v) == 0)
		{
			lookup.insert(v);

			const edge* e = predecessor[v];
			v = e->get_source();

			cycle->push_back(residual_graph->get_edge(e));
			*gamma = std::min(*gamma, e->get_capacity());
		}
	}

	return;
}

void algorithm::successive_shortest_path(
	const graph* full_graph,
	bool* minimum_cost_flow_found,
	double* minimum_cost_flow)
{
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> flow_per_edge;


	*minimum_cost_flow_found = false;
	*minimum_cost_flow = 0.0;

	// Schritt 1: Initialisierung des Graph
	// - Kanten mit negativen Kosten: voll auslasten
	// - Restliche Kanten: 0
	for(const edge* e : full_graph->get_edges())
	{
		double flow = 0.0;

		if(e->get_cost() < 0)
			flow = e->get_capacity();

		flow_per_edge.insert(std::make_pair(e, flow));
	}

	while(true)
	{
		// Schritt 2: Pseudo-Quellen und –Senken ermitteln
		// - Pseudo-Quellen: b(s) > b'(s)
		// - Pseudo-Senken: b(t) < b'(t)
		std::vector<const vertex*> pseudo_source, pseudo_target;
		std::map<const vertex*, double, compare_vertex_id> b_prim;

		for(const edge* e : full_graph->get_edges())
		{
			const double cost = e->get_cost();
			if(cost >= 0.0)
				continue;

			const double capacity = e->get_capacity();

			const vertex* source = e->get_source();
			const bool source_not_found = b_prim.count(source) == 0;

			if(source_not_found)
				b_prim.insert(std::make_pair(source, 0.0));
			b_prim[source] += capacity;

			const vertex* target = e->get_target();
			const bool target_not_found = b_prim.count(target) == 0;
			if(target_not_found)
				b_prim.insert(std::make_pair(target, 0.0));
			b_prim[target] += -capacity;
		}

		for(const vertex* v : full_graph->get_vertices())
		{
			const double balance = v->get_balance();
			const double balance_prim = (b_prim.count(v) > 0) ? b_prim[v] : 0.0;

			if(balance > balance_prim)
				pseudo_source.push_back(v);
			if(balance < balance_prim)
				pseudo_target.push_back(v);
		}

		// Schritt 3:
		// Wenn Pseudo-Quelle und erreichbare –Senke existiert:
		// Kürzesten Weg zwischen Pseudo-Quelle und –Senke in G^f
		// bzgl. c^f berechnen, sonst zu Schritt 6 gehen
		graph residual_graph;

		cc_create_residual_graph(
			full_graph,
			&flow_per_edge,
			&residual_graph );

		// Schritt 4:
		// Flusserhöhung durchführen und ggf. Pseudo-Quellen/-Senken entfernen

		// Schritt 5:
		// Gehen Sie zu Schritt 3
		break;
	}

	// Schritt 6:
	// Wenn Balancen ausgeglichen sind: kostenminimalen Fluss gefunden
	// Sonst: kein b-Fluss vorhanden
}

}
