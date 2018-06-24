#include <gtest/gtest.h>
#include <graph.h>
#include <graph_vertex.h>
#include <graph_edge.h>
#include <graph_comparer.h>
#include <unordered_map>

TEST(graph_vertex, std_map_test)
{
	std::map<const graph::vertex*, double, graph::compare_vertex_id> mapping;
	graph::vertex v0(0);
	graph::vertex v1(1);
	graph::vertex v2(2);
	graph::vertex v4(4);

	mapping.insert(std::make_pair(&v0, 0.0));
	mapping.insert(std::make_pair(&v1, 1.0));
	EXPECT_EQ(mapping.size(), 2);

	EXPECT_EQ(mapping.count(&v0), 1);
	EXPECT_EQ(mapping.count(&v2), 0);

	EXPECT_EQ(mapping[&v0], 0.0);
	EXPECT_EQ(mapping[&v1], 1.0);

	mapping[&v2] = 2.0;
	EXPECT_EQ(mapping[&v2], 2.0);

	mapping[&v1] += 3.0;
	EXPECT_EQ(mapping[&v1], 4.0);

	mapping[&v4] += 4.0;
	EXPECT_EQ(mapping[&v4], 4.0);

	return;
}

TEST(graph_edge, std_unordered_map_test)
{
	graph::edge e0, e1, e2, e3;
	graph::vertex v0(0), v1(1), v2(2);

	std::unordered_map<
		const graph::edge*,
		double,
		graph::undirected_edge_hash,
		graph::undirected_edge_equal> undirected_mapping;

	e0.set_source(&v0);
	e0.set_target(&v1);

	e1.set_source(&v1);
	e1.set_target(&v0);

	undirected_mapping[&e0] = 7.0;

	EXPECT_EQ(undirected_mapping[&e0], 7.0);
	EXPECT_EQ(undirected_mapping[&e1], 7.0);
	EXPECT_EQ(undirected_mapping.size(), 1);

	undirected_mapping[&e1] += 3.0;
	EXPECT_EQ(undirected_mapping[&e0], 10.0);
	EXPECT_EQ(undirected_mapping[&e1], 10.0);
	EXPECT_EQ(undirected_mapping.size(), 1);
}

TEST(graph_graph, return_edges)
{
	graph::graph g;
	graph::edge e0, e1;
	const graph::edge* e00, *e11;
	graph::vertex v0(0), v1(1);

	g.add_directed_edge(0, 1, 0.0, 0.0);

	e0.set_source(&v0);
	e0.set_target(&v1);

	e1.set_source(&v1);
	e1.set_target(&v0);

	e00 = g.get_edge(&e0, false);
	e11 = g.get_edge(&e1, false);

	EXPECT_EQ(e00, e11);
	EXPECT_EQ(e00->get_source()->get_id(), 0);
	EXPECT_EQ(e00->get_target()->get_id(), 1);

	e00 = g.get_edge(&e0, true);

	EXPECT_TRUE(e00 != nullptr);
	EXPECT_EQ(e00->get_source()->get_id(), 0);
	EXPECT_EQ(e00->get_target()->get_id(), 1);

	g.add_directed_edge(1, 0, 0.0, 0.0);

	e00 = g.get_edge(&e0, true);
	EXPECT_EQ(e00->get_source()->get_id(), 0);
	EXPECT_EQ(e00->get_target()->get_id(), 1);

	e11 = g.get_edge(&e1, true);
	EXPECT_EQ(e11->get_source()->get_id(), 1);
	EXPECT_EQ(e11->get_target()->get_id(), 0);

//	e00 = g.get_edge(&e0, false);
//	EXPECT_EQ(e00->get_source()->get_id(), 0);
//	EXPECT_EQ(e00->get_target()->get_id(), 1);

//	e11 = g.get_edge(&e1, false);
//	EXPECT_EQ(e11->get_source()->get_id(), 1);
//	EXPECT_EQ(e11->get_target()->get_id(), 0);

	return;
}
