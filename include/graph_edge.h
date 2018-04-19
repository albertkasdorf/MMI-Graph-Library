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
	const vertex* source(void) const;
	const vertex* target(void) const;

	void source(const vertex*);
	void target(const vertex*);

	bool twin_has(void) const;
	bool directed(void) const;
	const edge* twin(void) const;
	void twin(const edge*);

	bool weight_has(void) const;
	float weight(void) const;
	void weight(const float);

	std::size_t get_hash(void) const;

public:
	static std::size_t create_hash(const edge*);
};

}
