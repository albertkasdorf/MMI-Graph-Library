#include <graph_algorithm.h>

#include <deque>
#include <memory>

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

}
