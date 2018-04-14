#pragma once

#include <string>
#include <graph_files.h>

namespace graph
{
class graph;

class loader
{
public:
	loader();
	~loader();

public:
	void load(const files& file, graph& graph);

private:
	std::string file_name_get(const files& file);

	void load_adjacent_matrix(const std::string& file_name, graph& graph);
	void load_edge_list(const std::string& file_name, graph& graph);
	void load_edge_list_weighted(const std::string& file_name, graph& graph);
};

}
