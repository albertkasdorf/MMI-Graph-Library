#pragma once
#include <memory>
#include <vector>

namespace graph
{
class graph;
class vertex;

class algorithm
{
public:
	algorithm();
	~algorithm();

public:
	void breadth_first_search(
		const graph&, const vertex&, graph&);

	void connected_component(
		const graph&, std::vector<std::shared_ptr<graph>>&);
};

}
