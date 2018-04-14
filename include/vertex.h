#pragma once
#include <cstdint>

namespace graph
{

class vertex
{
private:
	std::uint32_t _id;

public:
	vertex(const std::uint32_t id) : _id(id) {}
	~vertex() {}

public:
	std::uint32_t id(void) const {return _id;}
};

}
