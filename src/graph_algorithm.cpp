#include <graph_algorithm.h>

#include <deque>

#include <vertex.h>
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
		auto vertex_current = l1.front();
		l1.pop_front();

		for(auto edge : graph_full.edge_get(vertex_current))
		{
			auto vertex_adjacent = graph_full.vertex_get(edge.target_id());

			auto l2_iter = l2.find(vertex_adjacent);
			if(l2_iter == std::end(l2))
			{
				l1.push_back(vertex_adjacent);
				l2.insert(vertex_adjacent);

				graph_sub.edge_add(edge);
			}
		}
	}
	return;
}

}
