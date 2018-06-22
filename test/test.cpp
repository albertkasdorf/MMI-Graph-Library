#include <gtest/gtest.h>
#include <graph_vertex.h>
#include <graph_comparer.h>
#include <graph.h>
#include <graph_loader.h>
#include <graph_algorithm.h>
#include <graph_vertex.h>
#include <graph_edge.h>
#include <graph_files.h>
#include <map>

TEST(always_true, expect_true)
{
	EXPECT_TRUE(true);
}

TEST(graph_algorithm, moore_bellman_ford)
{
	using namespace graph;

	graph::graph gg;
	graph::algorithm ga;
	std::map<const vertex*, const edge*, compare_vertex_id> predecessor;
	std::map<const vertex*, double> distances;

	const vertex* v0 = gg.add_vertex(nullptr, nullptr);
	const vertex* v1 = gg.add_vertex(nullptr, nullptr);
	const vertex* v2 = gg.add_vertex(nullptr, nullptr);

	gg.add_directed_edge(0, 1, -5.0, 0.0);
	gg.add_directed_edge(0, 2, -2.0, 0.0);
	gg.add_directed_edge(2, 1, -2.0, 0.0);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, nullptr);

	EXPECT_EQ(distances[v0], 0);
	EXPECT_EQ(distances[v1], -5.0);
	EXPECT_EQ(distances[v2], -2.0);

	EXPECT_EQ(predecessor[v0], nullptr);
	EXPECT_EQ(predecessor[v1]->get_source()->get_id(), v0->get_id());
	EXPECT_EQ(predecessor[v2]->get_source()->get_id(), v0->get_id());

	return;
}

TEST(graph_algorithm, moore_bellman_ford_from_slides)
{
	using namespace graph;

	graph::graph gg;
	graph::algorithm ga;
	std::map<const vertex*, const edge*, compare_vertex_id> predecessor;
	std::map<const vertex*, double> distances;

	const vertex* s = gg.add_vertex(nullptr, nullptr);
	const vertex* a = gg.add_vertex(nullptr, nullptr);
	const vertex* b = gg.add_vertex(nullptr, nullptr);
	const vertex* c = gg.add_vertex(nullptr, nullptr);
	const vertex* d = gg.add_vertex(nullptr, nullptr);

	gg.add_directed_edge(s->get_id(), a->get_id(), 7.0, 0.0);
	gg.add_directed_edge(s->get_id(), b->get_id(), 3.0, 0.0);
	gg.add_directed_edge(a->get_id(), c->get_id(), -5.0, 0.0);
	gg.add_directed_edge(b->get_id(), c->get_id(), 1.0, 0.0);
	gg.add_directed_edge(b->get_id(), d->get_id(), 1.0, 0.0);
	gg.add_directed_edge(c->get_id(), d->get_id(), 1.0, 0.0);

	ga.moore_bellman_ford(&gg, s, &predecessor, &distances, nullptr);

	EXPECT_EQ(distances[s], 0);
	EXPECT_EQ(distances[a], 7);
	EXPECT_EQ(distances[b], 3);
	EXPECT_EQ(distances[c], 2);
	EXPECT_EQ(distances[d], 3);

	EXPECT_EQ(predecessor[s], nullptr);
	EXPECT_EQ(predecessor[a]->get_source()->get_id(), s->get_id());
	EXPECT_EQ(predecessor[b]->get_source()->get_id(), s->get_id());
	EXPECT_EQ(predecessor[c]->get_source()->get_id(), a->get_id());
	EXPECT_EQ(predecessor[d]->get_source()->get_id(), c->get_id());

	return;
}

TEST(graph_algorithm, moore_bellman_ford_with_wege1)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v0 = nullptr;
	const graph::vertex* v2 = nullptr;

	std::map<const graph::vertex*, const graph::edge*, graph::compare_vertex_id> predecessor;
	std::map<const graph::vertex*, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::Wege1, gg, true);
	v0 = gg.get_vertex(0);
	v2 = gg.get_vertex(2);

	ga.moore_bellman_ford(&gg, v2, &predecessor, &distances, &negative_cycle_found);

	EXPECT_EQ(distances[v0], 6);
	EXPECT_FALSE(negative_cycle_found);

	return;
}

TEST(graph_algorithm, moore_bellman_ford_with_wege2)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v0 = nullptr;
	const graph::vertex* v2 = nullptr;

	std::map<const graph::vertex*, const graph::edge*, graph::compare_vertex_id> predecessor;
	std::map<const graph::vertex*, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::Wege2, gg, true);
	v0 = gg.get_vertex(0);
	v2 = gg.get_vertex(2);

	ga.moore_bellman_ford(&gg, v2, &predecessor, &distances, &negative_cycle_found);

	EXPECT_EQ(distances[v0], 2);
	EXPECT_FALSE(negative_cycle_found);

	return;
}

TEST(graph_algorithm, moore_bellman_ford_with_wege3)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v2 = nullptr;

	std::map<const graph::vertex*, const graph::edge*, graph::compare_vertex_id> predecessor;
	std::map<const graph::vertex*, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::Wege3, gg, true);
	v2 = gg.get_vertex(2);

	ga.moore_bellman_ford(&gg, v2, &predecessor, &distances, &negative_cycle_found);

	EXPECT_TRUE(negative_cycle_found);

	return;
}

//TEST(graph_algorithm, moore_bellman_ford_with_g_1_2_directed)
TEST(DISABLED_graph_algorithm, moore_bellman_ford_with_g_1_2_directed)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v0 = nullptr;
	const graph::vertex* v1 = nullptr;

	std::map<const graph::vertex*, const graph::edge*, graph::compare_vertex_id> predecessor;
	std::map<const graph::vertex*, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::G_1_2, gg, true);
	v0 = gg.get_vertex(0);
	v1 = gg.get_vertex(1);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, &negative_cycle_found);

	EXPECT_NEAR(distances[v1], 5.54417, 0.00001);
	EXPECT_FALSE(negative_cycle_found);

	return;
}

//TEST(graph_algorithm, moore_bellman_ford_with_g_1_2_undirected)
TEST(DISABLED_graph_algorithm, moore_bellman_ford_with_g_1_2_undirected)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v0 = nullptr;
	const graph::vertex* v1 = nullptr;

	std::map<const graph::vertex*, const graph::edge*, graph::compare_vertex_id> predecessor;
	std::map<const graph::vertex*, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::G_1_2, gg, false);
	v0 = gg.get_vertex(0);
	v1 = gg.get_vertex(1);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, &negative_cycle_found);

	EXPECT_NEAR(distances[v1], 2.36796, 0.00001);
	EXPECT_FALSE(negative_cycle_found);

	return;
}

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
	using namespace graph;

	edge e0, e1, e2, e3;
	vertex v0(0), v1(1), v2(2);

	std::unordered_map<
		const edge*,
		double,
		undirected_edge_hash,
		undirected_edge_equal> undirected_mapping;

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
	using namespace graph;

	graph::graph g;
	edge e0, e1;
	const edge* e00, *e11;
	vertex v0(0), v1(1);

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

TEST(graph_algorithm_successive_shortest_path, compute_b_prime)
{
	graph::graph gg;
	graph::algorithm ga;
	const graph::vertex* v0, *v1, *v2, *v3, *v4, *v5;

	std::unordered_map<
		const graph::edge*,
		double,
		graph::undirected_edge_hash,
		graph::undirected_edge_equal> flow_per_edge;
	std::map<const graph::vertex*, double, graph::compare_vertex_id> b_prime;

	gg.add_directed_edge(0, 1, 0.0, 0.0);
	gg.add_directed_edge(1, 2, 0.0, 0.0);
	gg.add_directed_edge(1, 3, 0.0, 0.0);
	gg.add_directed_edge(3, 4, 0.0, 0.0);
	gg.add_directed_edge(4, 1, 0.0, 0.0);
	gg.add_directed_edge(5, 4, 0.0, 0.0);

	v0 = gg.get_vertex(0);
	v1 = gg.get_vertex(1);
	v2 = gg.get_vertex(2);
	v3 = gg.get_vertex(3);
	v4 = gg.get_vertex(4);
	v5 = gg.get_vertex(5);

	flow_per_edge[gg.get_edge(v0, v1)] = 2.0;
	flow_per_edge[gg.get_edge(v1, v2)] = 5.0;
	flow_per_edge[gg.get_edge(v1, v3)] = 0.0;
	flow_per_edge[gg.get_edge(v3, v4)] = 3.0;
	flow_per_edge[gg.get_edge(v4, v1)] = 8.0;
	flow_per_edge[gg.get_edge(v5, v4)] = 5.0;

	ga.compute_b_prime(&gg, &flow_per_edge, &b_prime);

	EXPECT_EQ(b_prime[v0], 2.0);
	EXPECT_EQ(b_prime[v1], -5.0);
	EXPECT_EQ(b_prime[v2], -5.0);
	EXPECT_EQ(b_prime[v3], 3.0);
	EXPECT_EQ(b_prime[v4], 0.0);
	EXPECT_EQ(b_prime[v5], 5.0);
}

TEST(graph_algorithm_successive_shortest_path, generate_pseudo_source_target)
{

}



int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
