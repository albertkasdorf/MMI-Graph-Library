#pragma once
#include <cstdint>

namespace graph
{

class edge
{
private:
	std::uint32_t _source_id;
	std::uint32_t _target_id;
	float _weight;
	bool _directed;

public:
	edge(
		const std::uint32_t source_id,
		const std::uint32_t target_id,
		const float weight = 0.f,
		const bool directed = false)
		:
		_source_id(source_id),
		_target_id(target_id),
		_weight(weight),
		_directed(directed) { }
	edge(const edge& rhs) :
		_source_id(rhs.source_id()),
		_target_id(rhs.target_id()),
		_weight(rhs.weight()),
		_directed(rhs.directed()) { }
	~edge() { }

public:
	std::uint32_t source_id(void) const
	{
		return _source_id;
	}

	std::uint32_t target_id(void) const
	{
		return _target_id;
	}

	float weight(void) const
	{
		return _weight;
	}

	bool directed(void) const
	{
		return _directed;
	}
};

}
