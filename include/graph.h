#pragma once

#include <set>
#include <map>
#include <cstdint>
#include <vector>

namespace graph
{
class vertex;
class edge;

class graph
{
public:
	graph();
	~graph();

private:
	std::map<std::uint32_t, vertex*> vertices;
	std::vector<edge*> edges;
	std::map<std::uint32_t, std::map<std::uint32_t, std::vector<edge*>>> edges_map;

public:
	void vertex_add(std::uint32_t id){}
	void vertex_remove(std::uint32_t id) {}
	vertex vertex_get(std::uint32_t id) const;

	std::uint32_t vertex_count_get(void) const;

	void edge_add(
		const std::uint32_t source_id,
		const std::uint32_t target_id,
		const float weight = 0.f,
		const bool directed = false);

	void edge_add(const edge& edge_new);

	void edge_get(); // return iterator

	std::vector<edge> edge_get(const vertex& vertex) const;
	void edge_adjacent_get(const edge& edge);
	void edge_remove(const edge& edge);
};

}
