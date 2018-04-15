#include <iterator>
#include <map>

namespace graph
{
class vertex;

class vertex_iterator
{
private:
	std::map<std::uint32_t, vertex*>::const_iterator iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const vertex;
	using difference_type = std::ptrdiff_t;
	using pointer = const vertex*;
	using reference = const vertex&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	vertex_iterator() = default;
	explicit vertex_iterator(std::map<std::uint32_t, vertex*>::const_iterator iter);

	// Dereferencable.
	reference operator*() const;

	// Pre- and post-incrementable.
	vertex_iterator& operator++();
	vertex_iterator operator++(int);

	// Equality / inequality.
	bool operator==(const vertex_iterator& rhs);
	bool operator!=(const vertex_iterator& rhs);
};

}
