#include <graph_algorithm.h>

#include <deque>
#include <memory>
#include <queue>
#include <stack>
#include <cassert>

#include <graph_vertex.h>
#include <graph.h>
#include <comparer.h>

namespace graph
{

algorithm::algorithm()
{
}

algorithm::~algorithm()
{
}

void algorithm::breadth_first_search(
	const graph& graph_full,
	const vertex& vertex_start,
	graph& graph_sub)
{
	std::deque<vertex> l1;
	std::set<vertex, compare_vertex_id> l2;

	l1.push_back(vertex_start);
	l2.insert(vertex_start);

	while(!l1.empty())
	{
		const vertex vertex_current = l1.front();
		l1.pop_front();

		for(auto edge : graph_full.edge_get(vertex_current))
		{
			const std::uint32_t target_id = edge.target_id(vertex_current);
			const vertex vertex_adjacent = graph_full.vertex_get(target_id);

			const bool adjacent_vertex_processed = l2.count(vertex_adjacent) != 0;
			if(!adjacent_vertex_processed)
			{
				l1.push_back(vertex_adjacent);
				l2.insert(vertex_adjacent);

				graph_sub.add(edge);
			}
		}
	}
	return;
}

void algorithm::depth_first_search(
	const graph& graph_full,
	const vertex& vertex_start,
	graph& graph_sub)
{
	std::set<vertex, compare_vertex_id> lookup;

	depth_first_search_recursive(graph_full, vertex_start, lookup, graph_sub);
}

void algorithm::depth_first_search_recursive(
	const graph& graph_full,
	const vertex& vertex_to_expand,
	std::set<vertex, compare_vertex_id>& lookup,
	graph& graph_sub )
{
	lookup.insert(vertex_to_expand);

	for(auto edge : graph_full.edge_get(vertex_to_expand))
	{
		const std::uint32_t target_id = edge.target_id(vertex_to_expand);
		const vertex vertex_adjacent = graph_full.vertex_get(target_id);
		const bool vertex_found = lookup.count(vertex_adjacent);

		if(vertex_found)
			continue;

		graph_sub.add(edge);
		depth_first_search_recursive(graph_full, vertex_adjacent, lookup, graph_sub);
	}
}

void algorithm::connected_component_with_bfs(
	const graph& graph_full,
	std::vector<std::shared_ptr<graph>>& subgraphs)
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
	const graph& graph_full,
	std::vector<std::shared_ptr<graph>>& subgraphs)
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
	const graph& graph_full,
	std::vector<std::shared_ptr<graph>>& subgraphs,
	const std::function<void(const graph&, const vertex&, graph&)>& search_algorithm )
{
	std::set<vertex, compare_vertex_id> lookup;

	for(vertex_iterator vi_graph_full = graph_full.vertices_begin();
		vi_graph_full != graph_full.vertices_end();
		++vi_graph_full)
	{
		const vertex vertex_current = (*vi_graph_full);

		const bool vertex_found = lookup.count(vertex_current) != 0;
		if(vertex_found)
			continue;

		std::shared_ptr<graph> graph_sub = std::make_shared<graph>();

		search_algorithm(graph_full, vertex_current, *graph_sub);

		for(vertex_iterator vi_graph_sub = graph_sub->vertices_begin();
			vi_graph_sub != graph_sub->vertices_end();
			++vi_graph_sub)
		{
			const vertex vertex_from_sub_graph = *vi_graph_sub;
			lookup.insert(vertex_from_sub_graph);
		}

		subgraphs.push_back(graph_sub);
		continue;
	}
}

//
// Find the minimal spanning tree with the prim algorithm.
//
void algorithm::prim(
	const graph& graph_full, const vertex& vertex_start, graph& graph_mst)
{
	std::priority_queue<edge, std::vector<edge>, compare_edge_weight> queue;
	std::set<vertex, compare_vertex_id> vertices_visited;

	vertices_visited.insert(vertex_start);
	for(const edge edge : graph_full.edge_get(vertex_start))
	{
		queue.push(edge);
	}

	while(!queue.empty())
	{
		edge edge_current = queue.top();
		queue.pop();

		const vertex vertex_source = vertex(edge_current.source_id());
		const vertex vertex_target = vertex(edge_current.target_id());

		const bool source_visited = vertices_visited.count(vertex_source) != 0;
		const bool target_visited = vertices_visited.count(vertex_target) != 0;

		if(source_visited && target_visited)
		{
			continue;
		}

		graph_mst.add(edge_current);
		if(!source_visited)
		{
			vertices_visited.insert(vertex_source);
			for(const edge edge : graph_full.edge_get(vertex_source))
			{
				queue.push(edge);
			}
		}
		if(!target_visited)
		{
			vertices_visited.insert(vertex_target);
			for(const edge edge : graph_full.edge_get(vertex_target))
			{
				queue.push(edge);
			}
		}
	}
}

//
// Find the minimal spanning tree with the kruskal algorithm.
//
void algorithm::kruskal(const graph& graph_full, graph& graph_mst)
{
	std::priority_queue<edge, std::vector<edge>, compare_edge_weight> queue;

	for(const edge& edge : graph_full.edge_get())
	{
		queue.push(edge);
	}

	while(!queue.empty())
	{
		const edge edge_add = queue.top();
		queue.pop();

		graph_mst.add(edge_add);

		const bool graph_has_cycle =
				has_cycle(graph_mst, vertex(edge_add.source_id()));
		if(graph_has_cycle)
			graph_mst.remove(edge_add);

		continue;
	}

	return;
}

bool algorithm::has_cycle(const graph& graph_full, const vertex& vertex_start)
{
	std::set<vertex, compare_vertex_id> vertices_visited;
	std::set<edge, compare_edge_ids> edges_visited;
	std::stack<edge> stack;

	vertices_visited.insert(vertex_start);
	for(const edge edge : graph_full.edge_get(vertex_start))
	{
		stack.push(edge);
	}

	while(!stack.empty())
	{
		edge edge_current = stack.top();
		stack.pop();

		edges_visited.insert(edge_current);
		edges_visited.insert(edge_current.reverse_direction());

		const vertex vertex_source = vertex(edge_current.source_id());
		const vertex vertex_target = vertex(edge_current.target_id());

		const bool source_visited = vertices_visited.count(vertex_source) != 0;
		const bool target_visited = vertices_visited.count(vertex_target) != 0;

		if(source_visited && target_visited)
		{
			return true;
		}

		const vertex* vertex_next = nullptr;

		if(!source_visited)
			vertex_next = &vertex_source;
		else if(!target_visited)
			vertex_next = &vertex_target;
		else
			assert(false);

		vertices_visited.insert(*vertex_next);
		for(const edge edge_next : graph_full.edge_get(*vertex_next))
		{
			if(edges_visited.count(edge_next) != 0)
				continue;

			stack.push(edge_next);
		}
	}

	return false;
}


}
