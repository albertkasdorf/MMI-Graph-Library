#include <gtest/gtest.h>
#include <graph.h>
#include <graph_algorithm.h>
#include <graph_vertex.h>
#include <graph_edge.h>
#include <graph_loader.h>

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
	graph::graph gg;
	graph::algorithm ga;
	const graph::vertex* v0, *v1, *v2, *v3;
	std::vector<const graph::vertex*> pseudo_sources, pesudo_targets;
	std::map<const graph::vertex*, double, graph::compare_vertex_id> b_prime;

	v0 = gg.add_vertex(0, +7.0);
	v1 = gg.add_vertex(1, -3.0);
	v2 = gg.add_vertex(2, +8.0);
	v3 = gg.add_vertex(3, -5.0);

	b_prime[v0] = +7.0;
	b_prime[v1] = -5.0;
	b_prime[v2] = +3.0;
	b_prime[v3] = -3.0;

	ga.generate_pseudo_source_target(&gg, &b_prime, &pseudo_sources, &pesudo_targets);

	EXPECT_EQ(pseudo_sources.size(), 2);
	EXPECT_EQ(pesudo_targets.size(), 1);

	for(const graph::vertex* v : pseudo_sources)
	{
		if(v->get_id() == v1->get_id())
		{
			EXPECT_EQ(v, v1);
			EXPECT_EQ(v->get_balance(), v1->get_balance());
		}
		else if(v->get_id() == v2->get_id())
		{
			EXPECT_EQ(v, v2);
			EXPECT_EQ(v->get_balance(), v2->get_balance());
		}
		else
		{
			GTEST_FAIL();
		}
	}

	for(const graph::vertex* v : pesudo_targets)
	{
		if(v->get_id() == v3->get_id())
		{
			EXPECT_EQ(v, v3);
			EXPECT_EQ(v->get_balance(), v3->get_balance());
		}
		else
		{
			GTEST_FAIL();
		}
	}
}

TEST(graph_algorithm, successive_shortest_path_kostenminimal1)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	gl.load(graph::files::Kostenminimal1, gg);
	ga.successive_shortest_path(
		&gg, &minimum_cost_flow_found, &minimum_cost_flow);

	EXPECT_TRUE(minimum_cost_flow_found);
	EXPECT_EQ(minimum_cost_flow, 3.0);
}

TEST(graph_algorithm, successive_shortest_path_kostenminimal2)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	gl.load(graph::files::Kostenminimal2, gg);
	ga.successive_shortest_path(
		&gg, &minimum_cost_flow_found, &minimum_cost_flow);

	EXPECT_FALSE(minimum_cost_flow_found);
}

TEST(DISABLED_graph_algorithm, successive_shortest_path_kostenminimal3)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	gl.load(graph::files::Kostenminimal3, gg);
	ga.successive_shortest_path(
		&gg, &minimum_cost_flow_found, &minimum_cost_flow);

	EXPECT_TRUE(minimum_cost_flow_found);
	EXPECT_EQ(minimum_cost_flow, 1537);
}

TEST(graph_algorithm, successive_shortest_path_kostenminimal4)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	gl.load(graph::files::Kostenminimal4, gg);
	ga.successive_shortest_path(
		&gg, &minimum_cost_flow_found, &minimum_cost_flow);

	EXPECT_TRUE(minimum_cost_flow_found);
	EXPECT_EQ(minimum_cost_flow, 0.0);
}

TEST(graph_algorithm, successive_shortest_path_kostenminimal6)
{
	graph::graph gg;
	graph::loader gl;
	graph::algorithm ga;
	bool minimum_cost_flow_found = false;
	double minimum_cost_flow = 0.0;

	gl.load(graph::files::Kostenminimal6, gg);
	ga.successive_shortest_path(
		&gg, &minimum_cost_flow_found, &minimum_cost_flow);

	EXPECT_TRUE(minimum_cost_flow_found);
	EXPECT_EQ(minimum_cost_flow, 16.0);
}

TEST(graph_algorithm, successive_shortest_path_on_residual_kostenminimal)
{
	std::vector<std::tuple<graph::files, double, bool>> testcases = {
		std::make_tuple(graph::files::Kostenminimal1, 3.0, true),
		std::make_tuple(graph::files::Kostenminimal2, 0.0, false),
		std::make_tuple(graph::files::Kostenminimal4, 0.0, true),
		std::make_tuple(graph::files::Kostenminimal6, 16.0, true),
	};

	for(const std::tuple<graph::files, double, bool> testcase : testcases)
	{
		graph::graph gg;
		graph::loader gl;
		graph::algorithm ga;

		bool minimum_cost_flow_found = false;
		double minimum_cost_flow = 0.0;

		gl.load(std::get<0>(testcase), gg);

		ga.successive_shortest_path(
			&gg, &minimum_cost_flow_found, &minimum_cost_flow);

		EXPECT_EQ(minimum_cost_flow_found, std::get<2>(testcase));
		if(minimum_cost_flow_found)
			EXPECT_EQ(minimum_cost_flow, std::get<1>(testcase));
	}
}

TEST(DISABLED_graph_algorithm, successive_shortest_path_on_residual_kostenminimal3)
{
	std::vector<std::tuple<graph::files, double, bool>> testcases = {
		std::make_tuple(graph::files::Kostenminimal3, 1537, true)
	};

	for(const std::tuple<graph::files, double, bool> testcase : testcases)
	{
		graph::graph gg;
		graph::loader gl;
		graph::algorithm ga;

		bool minimum_cost_flow_found = false;
		double minimum_cost_flow = 0.0;

		gl.load(std::get<0>(testcase), gg);

		ga.successive_shortest_path(
			&gg, &minimum_cost_flow_found, &minimum_cost_flow);

		EXPECT_EQ(minimum_cost_flow_found, std::get<2>(testcase));
		if(minimum_cost_flow_found)
			EXPECT_EQ(minimum_cost_flow, std::get<1>(testcase));
	}
}

TEST(graph_kostenminimal, check_parallel_egdes)
{
	std::vector<graph::files> gfiles = {
		graph::files::Kostenminimal1,
		graph::files::Kostenminimal2,
		graph::files::Kostenminimal3,
		graph::files::Kostenminimal4,
		graph::files::Kostenminimal6
	};

	for(const graph::files file : gfiles)
	{
		graph::graph gg;
		graph::loader gl;

		gl.load(file, gg);

		for(const graph::vertex* v : gg.get_vertices())
		{
			std::set<std::uint32_t> lookup;

			for(const graph::edge* eofv : v->get_edges())
			{
				const std::uint32_t target_id = eofv->get_target()->get_id();

				EXPECT_TRUE(lookup.find(target_id) == lookup.end());
				lookup.insert(target_id);
			}
		}
	}
}

TEST(graph_kostenminimal, check_antiparallel_egdes)
{
	std::vector<graph::files> gfiles = {
		graph::files::Kostenminimal1,
		graph::files::Kostenminimal2,
		graph::files::Kostenminimal3,
		graph::files::Kostenminimal4,
		graph::files::Kostenminimal6
	};

	for(const graph::files file : gfiles)
	{
		graph::graph gg;
		graph::loader gl;

		gl.load(file, gg);

		for(const graph::vertex* v : gg.get_vertices())
		{
			for(const graph::edge* eOfv : v->get_edges())
			{
				const graph::vertex* target = eOfv->get_target();
				EXPECT_NE(v->get_id(), target->get_id());

				for(const graph::edge* e_of_target : target->get_edges())
				{
					EXPECT_NE(v->get_id(), e_of_target->get_target()->get_id());
				}
			}
		}
	}
}

TEST(graph_algorithm, create_residual_graph)
{
	graph::graph gg, ggres;
	graph::algorithm ga;
	const graph::edge* e01, *e12, *e13;
	std::unordered_map<
		const graph::edge*,
		double,
		graph::undirected_edge_hash,
		graph::undirected_edge_equal> flow_per_edge;

	gg.add_directed_edge(0, 1, 7, 5);
	gg.add_directed_edge(1, 2, 2, 2);
	gg.add_directed_edge(1, 3, -3, 7);

	e01 = gg.get_edge(gg.get_vertex(0), gg.get_vertex(1));
	e12 = gg.get_edge(gg.get_vertex(1), gg.get_vertex(2));
	e13 = gg.get_edge(gg.get_vertex(1), gg.get_vertex(3));

	flow_per_edge[e01] = 0.0;
	flow_per_edge[e12] = 2.0;
	flow_per_edge[e13] = 3.0;

	ga.create_residual_graph(&gg, &flow_per_edge, &ggres);

	EXPECT_EQ(ggres.get_edge_count(), 4);

	for(const graph::edge* e : ggres.get_edges())
	{
		if(e->get_source()->get_id() == 0 && e->get_target()->get_id() == 1)
		{
			EXPECT_EQ(e->get_capacity(), 5.0);
			EXPECT_EQ(e->get_cost(), 7.0);
		}
		else if(e->get_source()->get_id() == 2 && e->get_target()->get_id() == 1)
		{
			EXPECT_EQ(e->get_capacity(), 2.0);
			EXPECT_EQ(e->get_cost(), -2.0);
		}
		else if(e->get_source()->get_id() == 1 && e->get_target()->get_id() == 3)
		{
			EXPECT_EQ(e->get_capacity(), 4.0);
			EXPECT_EQ(e->get_cost(), -3.0);
		}
		else if(e->get_source()->get_id() == 3 && e->get_target()->get_id() == 1)
		{
			EXPECT_EQ(e->get_capacity(), 3.0);
			EXPECT_EQ(e->get_cost(), 3.0);
		}
		else
		{
			GTEST_FAIL();
		}
	}

	for(const graph::edge* e : ggres.get_edges())
	{
		if(e->get_source()->get_id() == 0 && e->get_target()->get_id() == 1)
		{
			const graph::edge* ecmp = gg.get_edge(e, false);
			EXPECT_EQ(ecmp, e01);
		}
		else if(e->get_source()->get_id() == 2 && e->get_target()->get_id() == 1)
		{
			const graph::edge* ecmp = gg.get_edge(e, false);
			EXPECT_EQ(ecmp, e12);
		}
		else if(e->get_source()->get_id() == 1 && e->get_target()->get_id() == 3)
		{
			const graph::edge* ecmp = gg.get_edge(e, false);
			EXPECT_EQ(ecmp, e13);
		}
		else if(e->get_source()->get_id() == 3 && e->get_target()->get_id() == 1)
		{
			const graph::edge* ecmp = gg.get_edge(e, false);
			EXPECT_EQ(ecmp, e13);
		}
		else
		{
			GTEST_FAIL();
		}
	}
}
