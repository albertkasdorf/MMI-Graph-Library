#pragma once
#include <cstdint>
#include <memory>

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
	float _weight;

public:
	edge(void);
	edge(const edge&) = delete;
	~edge();

public:
	const vertex* get_source(void) const;
	const vertex* get_target(void) const;

	void set_source(const vertex*);
	void set_target(const vertex*);

	bool has_twin(void) const;
	bool is_directed(void) const;
	const edge* get_twin(void) const;
	void set_twin(const edge*);

	bool has_weight(void) const;
	float get_weight(void) const;
	void set_weight(const float);

	std::size_t get_hash(void) const;

public:
	static std::size_t create_hash(const edge*);
};

}
