#pragma once
#include <cstdint>

namespace graph
{
class vertex;

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
		const bool directed = false);
	edge(const edge&);
	~edge();

public:
	std::uint32_t source_id(void) const;
	std::uint32_t source_id(const vertex&) const;

	std::uint32_t target_id(void) const;
	std::uint32_t target_id(const vertex&) const;

	float weight(void) const;
	bool directed(void) const;

	edge reverse_direction() const;

public:
	bool operator ==(const edge& rhs) const;
	bool operator !=(const edge& rhs) const;
};

}
