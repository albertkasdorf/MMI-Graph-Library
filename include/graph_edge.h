#pragma once
#include <cstdint>
#include <memory>
#include <cassert>

namespace graph
{
class vertex;

class edge
{
private:
	const vertex* _source;
	const vertex* _target;
	const edge* _twin;

	bool _has_weight;
	double _weight;

public:
	edge(void);
	edge(const edge&) = delete;
	virtual ~edge();

public:
	template<typename V = vertex>
	const V* get_source(void) const
	{
		const V* result = dynamic_cast<const V*>(_source);
		assert(result);
		return result;
	}

	template<typename V = vertex>
	const V* get_target(void) const
	{
		const V* result = dynamic_cast<const V*>(_target);
		assert(result);
		return result;
	}

	void set_source(const vertex*);
	void set_target(const vertex*);

	bool has_twin(void) const;
	bool is_directed(void) const;
	const edge* get_twin(void) const;
	void set_twin(const edge*);

	bool has_weight(void) const;
	double get_weight(void) const;
	void set_weight(const double);

	std::size_t get_hash(void) const;

public:
	static std::size_t create_hash(const edge*);
};

}
