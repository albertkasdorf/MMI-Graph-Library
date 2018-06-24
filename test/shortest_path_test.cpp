//#include <shortest_path.h>
#include <gtest/gtest.h>
#include <graph.h>
#include <graph_loader.h>
#include <graph_algorithm.h>
#include <graph_vertex.h>
#include <graph_edge.h>


TEST(graph_algorithm, moore_bellman_ford)
{
	graph::graph gg;
	graph::algorithm ga;
	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;

	const graph::vertex* v0 = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* v1 = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* v2 = gg.add_vertex(nullptr, nullptr);

	gg.add_directed_edge(0, 1, -5.0, 0.0);
	gg.add_directed_edge(0, 2, -2.0, 0.0);
	gg.add_directed_edge(2, 1, -2.0, 0.0);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, nullptr);

	EXPECT_EQ(distances[v0->get_id()], 0);
	EXPECT_EQ(distances[v1->get_id()], -5.0);
	EXPECT_EQ(distances[v2->get_id()], -2.0);

	EXPECT_EQ(predecessor[v0->get_id()], nullptr);
	EXPECT_EQ(predecessor[v1->get_id()]->get_source()->get_id(), v0->get_id());
	EXPECT_EQ(predecessor[v2->get_id()]->get_source()->get_id(), v0->get_id());

	return;
}

TEST(graph_algorithm, moore_bellman_ford_from_slides)
{
	graph::graph gg;
	graph::algorithm ga;
	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;

	const graph::vertex* s = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* a = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* b = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* c = gg.add_vertex(nullptr, nullptr);
	const graph::vertex* d = gg.add_vertex(nullptr, nullptr);

	gg.add_directed_edge(s->get_id(), a->get_id(), 7.0, 0.0);
	gg.add_directed_edge(s->get_id(), b->get_id(), 3.0, 0.0);
	gg.add_directed_edge(a->get_id(), c->get_id(), -5.0, 0.0);
	gg.add_directed_edge(b->get_id(), c->get_id(), 1.0, 0.0);
	gg.add_directed_edge(b->get_id(), d->get_id(), 1.0, 0.0);
	gg.add_directed_edge(c->get_id(), d->get_id(), 1.0, 0.0);

	ga.moore_bellman_ford(&gg, s, &predecessor, &distances, nullptr);

	EXPECT_EQ(distances[s->get_id()], 0);
	EXPECT_EQ(distances[a->get_id()], 7);
	EXPECT_EQ(distances[b->get_id()], 3);
	EXPECT_EQ(distances[c->get_id()], 2);
	EXPECT_EQ(distances[d->get_id()], 3);

	EXPECT_EQ(predecessor[s->get_id()], nullptr);
	EXPECT_EQ(predecessor[a->get_id()]->get_source()->get_id(), s->get_id());
	EXPECT_EQ(predecessor[b->get_id()]->get_source()->get_id(), s->get_id());
	EXPECT_EQ(predecessor[c->get_id()]->get_source()->get_id(), a->get_id());
	EXPECT_EQ(predecessor[d->get_id()]->get_source()->get_id(), c->get_id());

	return;
}

TEST(graph_algorithm, moore_bellman_ford_with_wege1)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v0 = nullptr;
	const graph::vertex* v2 = nullptr;

	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::Wege1, gg, true);
	v0 = gg.get_vertex(0);
	v2 = gg.get_vertex(2);

	ga.moore_bellman_ford(&gg, v2, &predecessor, &distances, &negative_cycle_found);

	EXPECT_EQ(distances[v0->get_id()], 6);
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

	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::Wege2, gg, true);
	v0 = gg.get_vertex(0);
	v2 = gg.get_vertex(2);

	ga.moore_bellman_ford(&gg, v2, &predecessor, &distances, &negative_cycle_found);

	EXPECT_EQ(distances[v0->get_id()], 2);
	EXPECT_FALSE(negative_cycle_found);

	return;
}

TEST(graph_algorithm, moore_bellman_ford_with_wege3)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	const graph::vertex* v2 = nullptr;

	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;
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

	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::G_1_2, gg, true);
	v0 = gg.get_vertex(0);
	v1 = gg.get_vertex(1);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, &negative_cycle_found);

	EXPECT_NEAR(distances[v1->get_id()], 5.54417, 0.00001);
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

	std::unordered_map<std::uint32_t, const graph::edge*> predecessor;
	std::unordered_map<std::uint32_t, double> distances;
	bool negative_cycle_found = false;

	gl.load(graph::files::G_1_2, gg, false);
	v0 = gg.get_vertex(0);
	v1 = gg.get_vertex(1);

	ga.moore_bellman_ford(&gg, v0, &predecessor, &distances, &negative_cycle_found);

	EXPECT_NEAR(distances[v1->get_id()], 2.36796, 0.00001);
	EXPECT_FALSE(negative_cycle_found);

	return;
}
