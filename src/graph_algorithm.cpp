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
#include <functional>

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
	std::unordered_map<std::uint32_t, const edge*>* predecessor,
	std::unordered_map<std::uint32_t, double>* distances,
	bool* negative_weights_found)
{
	std::unordered_map<std::uint32_t, const edge*> _predecessor;
	std::unordered_map<std::uint32_t, double> _distances;
	bool _negative_weights_found = false;

	if(!predecessor)
		predecessor = &_predecessor;
	if(!distances)
		distances = &_distances;
	if(!negative_weights_found)
		negative_weights_found = &_negative_weights_found;

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

		distances->insert(std::make_pair(vertex->get_id(), initial_distance));
		predecessor->insert(std::make_pair(vertex->get_id(), nullptr));

		unprocessed_vertices.insert(
			std::end(unprocessed_vertices), vertex);
	}

	// start algorithm with the start_vertex
	current_vertex = full_graph->get_vertex(start_vertex->get_id());
	current_vertex_distance = (*distances)[current_vertex->get_id()];

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
			const double target_vertex_distance = (*distances)[target_vertex->get_id()];

			const double new_distance =
				current_vertex_distance + explore_edge->get_weight();

			// If the new route is better, update the data
			if(new_distance < target_vertex_distance)
			{
				(*distances)[target_vertex->get_id()] = new_distance;
				(*predecessor)[target_vertex->get_id()] = explore_edge;
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
			const double next_vertex_distance = (*distances)[next_vertex->get_id()];

			if(next_vertex_distance <= current_vertex_distance)
			{
				current_vertex = next_vertex;
				current_vertex_distance = next_vertex_distance;
			}
		}
	}

	return;
}

void algorithm::moore_bellman_ford(
	const graph* g,
	const vertex* start_vertex,
	std::unordered_map<std::uint32_t, const edge*>* predecessor,
	std::unordered_map<std::uint32_t, double>* distances,
	bool* negative_cycle_found)
{
	std::unordered_map<std::uint32_t, const edge*> _predecessor;
	std::unordered_map<std::uint32_t, double> _distances;
	bool _negative_cycle_found = false;

	if(!predecessor)
		predecessor = &_predecessor;
	if(!distances)
		distances = &_distances;
	if(!negative_cycle_found)
		negative_cycle_found = &_negative_cycle_found;


	// Initialize distances and predecessor
	for(auto vertex : g->get_vertices())
	{
		double initial_distance = std::numeric_limits<double>::infinity();

		if(vertex->get_id() == start_vertex->get_id())
		{
			initial_distance = 0.0;
		}

		distances->insert(std::make_pair(vertex->get_id(), initial_distance));
		predecessor->insert(std::make_pair(vertex->get_id(), nullptr));
	}

	// Compute the distances and the predecessor
	for(std::uint32_t i = 0; i < g->get_vertex_count() - 1; ++i)
	{
		for(const edge* e : g->get_edges())
		{
			const vertex* source_vertex = e->get_source();
			const vertex* target_vertex = e->get_target();

			const double source_distance = (*distances)[source_vertex->get_id()];
			const double target_distance = (*distances)[target_vertex->get_id()];

			double cost = 0.0;

			if(e->has_cost())
				cost = e->get_cost();
			else
				if(e->has_weight())
					cost = e->get_weight();
				else
					assert(false);

			const double new_distance = source_distance + cost;

			if(new_distance < target_distance)
			{
				(*distances)[target_vertex->get_id()] = new_distance;
				(*predecessor)[target_vertex->get_id()] = e;
			}
		}
	}

	// Detect the negative cycle.
	for(const edge* e : g->get_edges())
	{
		const vertex* source_vertex = e->get_source();
		const vertex* target_vertex = e->get_target();

		const double source_distance = (*distances)[source_vertex->get_id()];
		const double target_distance = (*distances)[target_vertex->get_id()];

		double cost = 0.0;

		if(e->has_cost())
			cost = e->get_cost();
		else
			if(e->has_weight())
				cost = e->get_weight();
			else
				assert(false);

		const double new_distance = source_distance + cost;

		if(new_distance < target_distance)
		{
			*negative_cycle_found = true;
			break;
		}
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
		create_residual_graph(full_graph, flow_per_edge, &residual_graph);

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

	if(source_vertex == nullptr || target_vertex == nullptr)
		return;

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
	const bool show_viz = false;

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

	if(!b_flow_exist)
		return;

	viz_flow(show_viz, &flow_per_edge);

	while(true)
	{
		// Schritt 2:
		// Bestimmung des Residualgraphen, der Residualkapazitäten und der
		// Residualkosten.
		graph residual_graph;

		create_residual_graph(full_graph, &flow_per_edge, &residual_graph);

		viz_graph(show_viz, "Residual", &residual_graph);


		// Schritt 3:
		// Konstruieren eines f-augmentierenden Zykels Z in G f mit negativen Kosten.
		// Falls keiner existiert: STOPP
		std::list<const edge*> cycle;
		double gamma = 0.0;

		cc_find_negative_cycle(&residual_graph, &flow_per_edge, &cycle, &gamma);

		if(cycle.empty())
			break;

		viz_cycle(show_viz, &cycle, gamma);


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

		viz_flow(show_viz, &flow_per_edge);


		// Schritt 5:
		// Ab Schritt 2 wiederholen.
	}

	*minimum_cost_flow_found = true;
	for(const edge* e : full_graph->get_edges())
	{
		const double flow = flow_per_edge[e];
		const double cost = e->get_cost();

		*minimum_cost_flow += (flow * cost);
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
	double sum_of_super_source_flow = 0.0;
	double sum_of_super_target_flow = 0.0;
	std::vector<const vertex*> src_or_tgt_vert;

	// Compute the flow from/to the super source/target
	// Also save the vertices that provide/reveice flow
	for(auto v : b_flow_graph.get_vertices())
	{
		assert(v->has_balance());
		const double balance = v->get_balance();

		if(balance == 0.0)
			continue;

		if(balance > 0.0)
		{
			sum_of_super_source_flow += balance;
		}
		else // if(balance < 0.0)
		{
			sum_of_super_target_flow += balance;
		}

		src_or_tgt_vert.push_back(v);
	}

	// Create two new vertices in the b_flow_graph for the super source/target
	const vertex* super_source =
		b_flow_graph.add_vertex(nullptr, &sum_of_super_source_flow);
	const vertex* super_target =
		b_flow_graph.add_vertex(nullptr, &sum_of_super_target_flow);

	// Add edges from the super source/target to the provide/reveice flow vertices.
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

	*b_flow_exist = (sum_of_super_source_flow == maximum_flow);

	// Translate the edges of b_flow_graph to full_graph
	for(const edge* e : full_graph->get_edges())
	{
		const double flow = flow_per_edge_internal[e];
		flow_per_edge->insert(std::make_pair(e, flow));
	}

	return;
}

void algorithm::create_residual_graph(
	const graph* g,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	graph* residual_graph)
{
	for(const vertex* v : g->get_vertices())
	{
		residual_graph->add_vertex(v);
	}

	for(const edge* e : g->get_edges())
	{
		add_residual_edge(e, flow_per_edge, residual_graph);
	}

	return;
}

void algorithm::add_residual_edge(
	const edge* original_edge,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	graph* residual_graph)
{
	const std::uint32_t source_id = original_edge->get_source()->get_id();
	const std::uint32_t target_id = original_edge->get_target()->get_id();

	// u(e)
	double edge_capacity = 0.0;
	if(original_edge->has_capacity())
		edge_capacity = original_edge->get_capacity();
	else
		if(original_edge->has_weight())
			edge_capacity = original_edge->get_weight();
		else
			assert(false);
	// f(e)
	const double edge_value = flow_per_edge->at(original_edge);
	// c(e)
	double edge_cost = 0.0;
	if(original_edge->has_cost())
		edge_cost = original_edge->get_cost();
	else
		if(original_edge->has_weight())
			edge_cost = original_edge->get_weight();
		else
			assert(false);

	// residual_capacity
	// Forward: u^f(e) = u(e) - f(e)
	const double uf_forward_edge = edge_capacity - edge_value;
	// Backward: u^f(e) = f(e)
	const double uf_backward_edge = edge_value;

	if(uf_forward_edge > 0.0)
	{
		std::shared_ptr<edge> forward_edge = original_edge->create_copy();

		forward_edge->set_capacity(uf_forward_edge);
		forward_edge->set_cost(edge_cost);
		forward_edge->set_source(residual_graph->get_vertex(source_id));
		forward_edge->set_target(residual_graph->get_vertex(target_id));

		// Insert forward edge
		residual_graph->add_edge(forward_edge.get());
	}

	if(uf_backward_edge > 0.0)
	{
		std::shared_ptr<edge> backward_edge = original_edge->create_copy();

		backward_edge->set_capacity(uf_backward_edge);
		backward_edge->set_cost(-edge_cost);
		backward_edge->set_source(residual_graph->get_vertex(target_id));
		backward_edge->set_target(residual_graph->get_vertex(source_id));

		// Insert backward edge
		residual_graph->add_edge(backward_edge.get());
	}
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

	// Create a start_vertex and connect all other vertices with the start_vertex
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

				// In the last round of Bellman-Ford add all changed vertices.
				// They can be potentiall part of the negative cycle
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

		// Follow vertex_count-times the predecessor.
		// After that we are with v in the negative cycle.
		for(std::uint32_t i = 0; i < vertex_count; ++i)
		{
			const edge* e = predecessor[v];
			v = e->get_source();
			continue;
		}

		// Add from v all vertices to the cycle list until we came back.
		// Compute also gamma.
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

//
// Not working
//
//void algorithm::successive_shortest_path(
//	const graph* full_graph,
//	bool* minimum_cost_flow_found,
//	double* minimum_cost_flow)
//{
//	const bool show_viz = false;

//	std::unordered_map<
//		const edge*,
//		double,
//		undirected_edge_hash,
//		undirected_edge_equal> flow_per_edge;


//	*minimum_cost_flow_found = false;
//	*minimum_cost_flow = 0.0;

//	// Schritt 1: Initialisierung des Graph
//	// - Kanten mit negativen Kosten: voll auslasten
//	// - Restliche Kanten: 0
//	for(const edge* e : full_graph->get_edges())
//	{
//		double flow = 0.0;

//		if(e->get_cost() < 0.0)
//			flow = e->get_capacity();

//		flow_per_edge.insert(std::make_pair(e, flow));
//		assert(flow_per_edge[e] >= 0.0);
//	}

//	viz_flow(show_viz, &flow_per_edge);

//	while(true)
//	{
//		// Schritt 2: Pseudo-Quellen und –Senken ermitteln
//		// - Pseudo-Quellen: b(s) > b'(s)
//		// - Pseudo-Senken: b(t) < b'(t)
//		std::vector<const vertex*> pseudo_source, pseudo_target;
//		std::map<const vertex*, double, compare_vertex_id> b_prime;


//		// Compute values for b_prime
//		compute_b_prime(full_graph, &flow_per_edge, &b_prime);

//		// Create two lists of pseudo_source(s) and pseudo_target(s)
//		generate_pseudo_source_target(
//			full_graph, &b_prime, &pseudo_source, &pseudo_target);

//		viz_pseudo_source_target(show_viz, &pseudo_source, &pseudo_target);

//		// Pseudo balance b'(v) and vertex balance b(v) are equal.
//		if(pseudo_source.empty() && pseudo_target.empty())
//		{
//			*minimum_cost_flow_found = true;
//			break;
//		}

//		// Only one source or one target, can not create b-flow.
//		if(pseudo_source.empty() || pseudo_target.empty())
//		{
//			*minimum_cost_flow_found = false;
//			break;
//		}

//		// Schritt 3:
//		// Wenn Pseudo-Quelle und erreichbare –Senke existiert:
//		// Kürzesten Weg zwischen Pseudo-Quelle und –Senke in G^f
//		// bzgl. c^f berechnen, sonst zu Schritt 6 gehen
//		graph residual_graph;

//		create_residual_graph(full_graph, &flow_per_edge, &residual_graph);

//		viz_graph(show_viz, "Residual", &residual_graph);

//		std::list<std::pair<const edge*, bool>> path_of_change;
//		double min_capacity = std::numeric_limits<double>::infinity();
//		std::pair<const vertex*, const vertex*> path_start_end = std::make_pair(nullptr, nullptr);

//		// Searching for a (s,t)-path.
//		for(const vertex* source : pseudo_source)
//		{
//			std::unordered_map<std::uint32_t, const edge*> predecessor;
//			std::unordered_map<std::uint32_t, double> distances;

//			moore_bellman_ford(
//				&residual_graph, source, &predecessor, &distances, nullptr);

//			for(const vertex* target : pseudo_target)
//			{
//				if(distances[target->get_id()] == std::numeric_limits<double>::infinity())
//					continue;

//				std::list<const edge*> path;
//				const edge* e_to_pred = nullptr;

//				e_to_pred = predecessor[target->get_id()];
//				while(e_to_pred != nullptr)
//				{
//					path.push_front(e_to_pred);
//					e_to_pred = predecessor[e_to_pred->get_source()->get_id()];
//				}

//				// There exist no path
//				if(path.empty())
//					continue;

//				// Path to the source does not exist.
//				if(path.front()->get_source()->get_id() != source->get_id())
//					continue;

//				// Translate residual_graph edges to full_graph edges
//				for(const edge* e : path)
//				{
//					const edge* e_from_full_graph = full_graph->get_edge(e, false);
//					const bool same_direction =
//						(e->get_source()->get_id() == e_from_full_graph->get_source()->get_id());

//					path_of_change.push_back(
//						std::make_pair(e_from_full_graph, same_direction));
//					min_capacity = std::min(
//						min_capacity, e_from_full_graph->get_capacity());
//				}
//				path_start_end = std::make_pair(
//					full_graph->get_vertex(source->get_id()),
//					full_graph->get_vertex(target->get_id()));

//				break;
//			}

//			// Hint:
//			// If we do not found a flow from the source to a target,
//			// we will never found a flow.
//			if(path_of_change.empty())
//				continue;
//			else
//				break;
//		}

//		// There is no way between the source(s) and target(s).
//		if(path_of_change.empty())
//		{
//			*minimum_cost_flow_found = false;
//			break;
//		}

//		// Schritt 4:
//		// Flusserhöhung durchführen und ggf. Pseudo-Quellen/-Senken entfernen
//		const double diff_b_and_b_prime =
//			path_start_end.first->get_balance() - b_prime[path_start_end.first];
//		const double diff_b_prime_and_b =
//			b_prime[path_start_end.second] - path_start_end.second->get_balance();
//		const double gamma =
//			std::min(min_capacity, std::min(diff_b_and_b_prime, diff_b_prime_and_b));

//		viz_flow_change(show_viz, &path_of_change, gamma);

//		for(std::pair<const edge*, bool> kvp : path_of_change)
//		{
//			const double sign = (kvp.second) ? 1.0 : -1.0;
//			const double flow = flow_per_edge[kvp.first];

//			flow_per_edge[kvp.first] = flow + (gamma * sign);

//			assert(flow_per_edge[kvp.first] >= 0.0);
//		}

//		viz_flow(show_viz, &flow_per_edge);

//		// Schritt 5:
//		// Gehen Sie zu Schritt 3
//	}

//	// Schritt 6:
//	// Wenn Balancen ausgeglichen sind: kostenminimalen Fluss gefunden
//	// Sonst: kein b-Fluss vorhanden
//	if(*minimum_cost_flow_found)
//	{
//		for(const edge* e : full_graph->get_edges())
//		{
//			const double flow = flow_per_edge[e];
//			const double cost = e->get_cost();

//			*minimum_cost_flow += (flow * cost);
//		}
//	}

//	return;
//}

void algorithm::successive_shortest_path(
	const graph* g, bool* minimum_cost_flow_found, double* minimum_cost_flow)
{
	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> flow_per_edge;
	std::map<std::uint32_t, double> b_of_v, b_prime_of_v;
	std::vector<std::uint32_t> pseudo_sources, pseudo_targets;
	graph residual_graph;


	// Schritt 1: Initialisierung des Graph
	// - Kanten mit negativen Kosten: voll auslasten
	// - Restliche Kanten: 0
	for(const edge* e : g->get_edges())
	{
		const double cost = e->get_cost();
		const double capacity = e->get_capacity();
		const double flow = (cost < 0.0) ? capacity : 0.0;

		flow_per_edge[e] = flow;
	}


	// Schritt 2: Pseudo-Quellen und –Senken ermitteln
	// - Pseudo-Quellen: b(s) > b'(s)
	// - Pseudo-Senken: b(t) < b'(t)
	{
		std::map<const vertex*, double, compare_vertex_id> b_prime;
		std::vector<const vertex*> _pseudo_source, _pseudo_target;

		compute_b_prime(g, &flow_per_edge, &b_prime);

		for(auto kvp : b_prime)
			b_prime_of_v[kvp.first->get_id()] = kvp.second;

		generate_pseudo_source_target(
			g, &b_prime, &_pseudo_source, &_pseudo_target);

		for(const vertex* v : _pseudo_source)
			pseudo_sources.push_back(v->get_id());
		for(const vertex* v : _pseudo_target)
			pseudo_targets.push_back(v->get_id());
	}
	for(const vertex* v : g->get_vertices())
		b_of_v[v->get_id()] = v->get_balance();


	// Schritt 3:
	// Wenn Pseudo-Quelle und erreichbare –Senke existiert:
	// Kürzesten Weg zwischen Pseudo-Quelle und –Senke in G^f
	// bzgl. c^f berechnen, sonst zu Schritt 6 gehen
	create_residual_graph(g, &flow_per_edge, &residual_graph);

	while(true)
	{
		// Pseudo balance b'(v) and vertex balance b(v) are equal.
		if(pseudo_sources.empty() && pseudo_targets.empty())
		{
			*minimum_cost_flow_found = true; break;
		}

		// Only sources or targets left, can not create b-flow.
		if(pseudo_sources.empty() || pseudo_targets.empty())
		{
			*minimum_cost_flow_found = false; break;
		}

		// Searching for a (s,t)-path.
		std::list<const edge*> st_path;

		for(const std::uint32_t pseudo_source : pseudo_sources)
		{
			std::unordered_map<std::uint32_t, const edge*> predecessor;
			std::unordered_map<std::uint32_t, double> distances;

			moore_bellman_ford(
				&residual_graph,
				residual_graph.get_vertex(pseudo_source),
				&predecessor,
				&distances,
				nullptr);

			for(const std::uint32_t pseudo_target : pseudo_targets)
			{
				if(distances[pseudo_target] == std::numeric_limits<double>::infinity())
					continue;

				const edge* e_to_pred = predecessor[pseudo_target];
				while(e_to_pred != nullptr)
				{
					st_path.push_front(e_to_pred);
					e_to_pred = predecessor[e_to_pred->get_source()->get_id()];
				}

				break;
			}

			break;
		}

		// We have source(s) and target(s), but no path.
		// => Minimum cost flow does not exist.
		if(st_path.empty())
		{
			*minimum_cost_flow_found = false; break;
		}

		// Schritt 4:
		// Flusserhöhung durchführen und ggf. Pseudo-Quellen/-Senken entfernen
		double min_capacity = std::numeric_limits<double>::infinity();
		for(const edge* e : st_path)
			min_capacity = std::min(min_capacity, e->get_capacity());

		const std::uint32_t source_id = st_path.front()->get_source()->get_id();
		const double src_b_diff = b_of_v[source_id] - b_prime_of_v[source_id];

		const std::uint32_t target_id = st_path.back()->get_target()->get_id();
		const double tgt_b_diff = b_prime_of_v[target_id] - b_of_v[target_id];

		const double gamma = std::min(min_capacity, std::min(src_b_diff, tgt_b_diff));

		// Change flow throw the st_path
		for(const edge* e : st_path)
		{
			assert(flow_per_edge.count(e) == 1);
			auto iter = flow_per_edge.find(e);
			assert(iter != flow_per_edge.end());

			const double flow_old = iter->second;
			const edge* original_edge = iter->first;
			const bool same_direction =
				(original_edge->get_source()->get_id() == e->get_source()->get_id());

			const double sign = (same_direction) ? 1.0 : -1.0;
			const double flow_new = flow_old + (sign * gamma);

			flow_per_edge[e] = flow_new;
		}

		// Change b_prime of source and target
		// Possibly remove pseudo_sources/pseude_targets
		b_prime_of_v[source_id] += gamma;
		b_prime_of_v[target_id] -= gamma;

		const bool remove_source = (b_of_v[source_id] == b_prime_of_v[source_id]);
		const bool remove_target = (b_of_v[target_id] == b_prime_of_v[target_id]);

		if(remove_source)
			pseudo_sources.erase(std::find(
				std::begin(pseudo_sources), std::end(pseudo_sources), source_id));
		if(remove_target)
			pseudo_targets.erase(std::find(
				std::begin(pseudo_targets), std::end(pseudo_targets), target_id));

		// Change residual graph
		for(const edge* e : st_path)
		{
			assert(flow_per_edge.count(e) == 1);
			auto iter = flow_per_edge.find(e);
			assert(iter != flow_per_edge.end());
			const edge* original_edge = iter->first;

			//----
			residual_graph.remove_edges(e->get_source(), e->get_target());
			e = nullptr;

			//----
			add_residual_edge(original_edge, &flow_per_edge, &residual_graph);
		}

		// Schritt 5:
		// Gehen Sie zu Schritt 3
		continue;
	}

	// Schritt 6:
	// Wenn Balancen ausgeglichen sind: kostenminimalen Fluss gefunden
	// Sonst: kein b-Fluss vorhanden
	if(*minimum_cost_flow_found)
	{
		*minimum_cost_flow = 0.0;

		for(const edge* e : g->get_edges())
		{
			const double flow = flow_per_edge[e];
			const double cost = e->get_cost();

			*minimum_cost_flow += (flow * cost);
		}
	}

	return;
}

void algorithm::generate_pseudo_source_target(
	const graph* g,
	std::map<const vertex*,double,compare_vertex_id>* b_prime,
	std::vector<const vertex*>* pseudo_sources,
	std::vector<const vertex*>* pesudo_targets)
{
	// s: b(s) - b'(s) > 0
	// t: b(t) - b'(t) < 0
	// n: b(t) - b'(t) = 0
	for(const vertex* v : g->get_vertices())
	{
		if(v->get_balance() > (*b_prime)[v])
			pseudo_sources->push_back(v);

		if(v->get_balance() < (*b_prime)[v])
			pesudo_targets->push_back(v);
	}
}

void algorithm::compute_b_prime(
	const graph* g,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge,
	std::map<
		const vertex*,
		double,
		compare_vertex_id>* b_prime)
{
	// Compute values for b_prime
	// δ+ outgoing edges
	// δ- incoming edges
	// b'(v) = sum[e∈δ+](f(e)) - sum[e∈δ-](f(e))
	for(const edge* e : g->get_edges())
	{
		const double flow = flow_per_edge->at(e);

		// Outgoing edge
		const vertex* source = e->get_source();
		(*b_prime)[source] += flow;

		// Incoming edge
		const vertex* target = e->get_target();
		(*b_prime)[target] += -flow;
	}
}

void algorithm::viz_flow(
	const bool show_viz,
	const std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal>* flow_per_edge)
{
	if(!show_viz)
		return;

	std::cout << "Flow: ";
	for(auto kvp : *flow_per_edge)
	{
		std::cout << kvp.first->get_source()->get_id() << "->";
		std::cout << kvp.first->get_target()->get_id() << ", ";
		std::cout << kvp.second << "; ";
	}
	std::cout << std::endl << std::endl;
}

void algorithm::viz_graph(
	const bool show_viz, const char* graph_type, const graph* graph)
{
	if(!show_viz)
		return;

	std::cout << graph_type << ": ";
	for(const edge* e : graph->get_edges())
	{
		std::cout << e->get_source()->get_id() << "->" << e->get_target()->get_id();
		std::cout << ", " << e->get_capacity() << "/" << e->get_cost() << "; ";
	}
	std::cout << std::endl << std::endl;
}

void algorithm::viz_pseudo_source_target(
	const bool show_viz,
	const std::vector<const vertex*>* pseudo_source,
	const std::vector<const vertex*>* pseudo_target)
{
	if(!show_viz)
		return;

	std::cout << "Pseudo-Source: ";
	for(const vertex* v : *pseudo_source)
		std::cout << v->get_id() << ", ";
	std::cout << "\nPseudo-Target: ";
	for(const vertex* v : *pseudo_target)
		std::cout << v->get_id() << ", ";
	std::cout << "\n\n";
}

void algorithm::viz_flow_change(
	const bool show_viz,
	const std::list<std::pair<const edge*, bool>>* path_of_change,
	const double gamma)
{
	if(!show_viz)
		return;

	std::cout << "Change flow between ";
	for(std::pair<const edge*, bool> kvp : *path_of_change)
	{
		std::cout << "(" << kvp.first->get_source()->get_id() << ",";
		std::cout << kvp.first->get_target()->get_id() << "), ";
	}
	std::cout << "with gamma=" << gamma << ".\n\n";
	std::cout.flush();
}

void algorithm::viz_cycle(
	const bool show_viz,
	const std::list<const edge*>* cycle,
	const double gamma)
{
	if(!show_viz)
		return;

	std::cout << "Change flow in cycle ";
	for(const edge* e : *cycle)
	{
		std::cout << "(" << e->get_source()->get_id() << ",";
		std::cout << e->get_target()->get_id() << "), ";
	}
	std::cout << "with gamma=" << gamma << ".\n\n";
	std::cout.flush();
}


}
