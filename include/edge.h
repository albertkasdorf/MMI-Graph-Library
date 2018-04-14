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
	edge(const std::uint32_t source_id, const std::uint32_t target_id) :
		_source_id(source_id), _target_id(target_id)
	{
	}

	~edge()
	{
	}

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
