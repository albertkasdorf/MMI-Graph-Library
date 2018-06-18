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

	std::unique_ptr<double> _weight;
	std::unique_ptr<double> _cost;
	std::unique_ptr<double> _capacity;

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

	bool has_cost(void) const;
	double get_cost(void) const;
	void set_cost(const double);

	bool has_capacity(void) const;
	double get_capacity(void) const;
	void set_capacity(const double);

	std::shared_ptr<edge> create_copy(void) const;

	std::size_t get_hash(void) const;

public:
	static std::size_t create_hash(const edge*);
};

}
