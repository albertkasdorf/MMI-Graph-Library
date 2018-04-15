#include <graph_loader.h>

#include <cassert>
#include <fstream>

#include <graph.h>
#include <graph_edge.h>
#include <graph_vertex.h>


namespace graph
{

loader::loader()
{
}

loader::~loader()
{
}

std::string loader::file_name_get(const files& graph_file)
{
	std::string file_path = "/home/albert/Documents/Studium/MMI-SS18/graphen/";
	std::string file_name;

	switch(graph_file)
	{
	case files::Graph1: file_name = "Graph1.txt"; break;
	case files::Graph2: file_name = "Graph2.txt"; break;
	case files::Graph3: file_name = "Graph3.txt"; break;
	case files::Graph4: file_name = "Graph4.txt"; break;
	case files::G_1_2: file_name = "G_1_2.txt"; break;
	case files::G_1_20: file_name = "G_1_20.txt"; break;
	case files::G_1_200: file_name = "G_1_200.txt"; break;
	case files::G_10_20: file_name = "G_10_20.txt"; break;
	case files::G_10_200: file_name = "G_10_200.txt"; break;
	case files::G_100_200: file_name = "G_100_200.txt"; break;
	default: assert(false);
	}

	return file_path + file_name;
}

void loader::load(const files& graph_file, graph& graph)
{
	std::string file_name = file_name_get(graph_file);

	switch(graph_file)
	{
	case files::Graph1:
		load_adjacent_matrix(file_name, graph);
		break;
	case files::Graph2:
	case files::Graph3:
	case files::Graph4:
		load_edge_list(file_name, graph);
		break;
	case files::G_1_2:
	case files::G_1_20:
	case files::G_1_200:
	case files::G_10_20:
	case files::G_10_200:
	case files::G_100_200:
		load_edge_list_weighted(file_name, graph);
		break;
	default: assert(false);
	}
}

void loader::load_adjacent_matrix(const std::string& file_name, graph& graph)
{
	const float edge_weight = 0.f;
	const bool edge_directed = false;

	std::fstream fs;
	std::size_t vertex_count = 0;

	fs.open(file_name.c_str());
	fs >> vertex_count;

	// create vertices
	for(std::uint32_t i = 0; i < vertex_count; ++i)
		graph.add(vertex(i));

	// create edges
	for(auto row = 0; row < vertex_count; row++)
	{
		for(auto col = 0; col < vertex_count; col++)
		{
			bool adjacent = {};

			fs >> adjacent;
			if(adjacent && col >= row)
			{
				graph.add(edge(row, col, edge_weight, edge_directed));
			}
		}
	}

	assert(graph.vertex_count_get() == vertex_count);
}

void loader::load_edge_list(
	const std::string& file_name, graph& graph)
{
	const float edge_weight = 0.f;
	const bool edge_directed = false;

	std::fstream fs;
	std::uint32_t vertex_count = 0;

	fs.open(file_name.c_str());
	fs >> vertex_count;

	// create vertices
	for(std::uint32_t i = 0; i < vertex_count; ++i)
		graph.add(vertex(i));

	// create edges
	while(true)
	{
		std::uint32_t source_id = {}, target_id = {};

		fs >> source_id >> target_id;
		if(fs.eof())
			break;

		graph.add(edge(source_id, target_id, edge_weight, edge_directed));
	}

	assert(graph.vertex_count_get() == vertex_count);
}

void loader::load_edge_list_weighted(
	const std::string& file_name, graph& graph)
{
	const bool edge_directed = false;

	std::fstream fs;
	std::uint32_t vertex_count = 0;

	fs.open(file_name.c_str());
	fs >> vertex_count;

	// create vertices
	for(std::uint32_t i = 0; i < vertex_count; ++i)
		graph.add(vertex(i));

	// create edges
	while(true)
	{
		std::uint32_t source_id = {}, target_id = {};
		float weight = {};

		fs >> source_id >> target_id >> weight;
		if(fs.eof())
			break;

		graph.add(edge(source_id, target_id, weight, edge_directed));
	}

	assert(graph.vertex_count_get() == vertex_count);
}

}
