#pragma once
#include <iterator>
#include <map>
#include <vector>
#include <memory>

namespace graph
{
class vertex;
class edge;

//
// http://anderberg.me/2016/07/04/c-custom-iterators/
//
class vertex_iterator
{
private:
	std::map<std::size_t, std::shared_ptr<vertex>>::const_iterator iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const vertex;
	using difference_type = std::ptrdiff_t;
	using pointer = const vertex*;
	using reference = const vertex&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	vertex_iterator() = default;
	explicit vertex_iterator(
		std::map<std::size_t, std::shared_ptr<vertex>>::const_iterator);

	// Dereferencable.
	pointer operator*() const;

	// Pre- and post-incrementable.
	vertex_iterator& operator++();
	vertex_iterator operator++(int);

	// Equality / inequality.
	bool operator==(const vertex_iterator&);
	bool operator!=(const vertex_iterator&);
};

class edge_iterator
{
private:
	std::vector<const edge*>::const_iterator iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const edge;
	using difference_type = std::ptrdiff_t;
	using pointer = const edge*;
	using reference = const edge&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	edge_iterator() = default;
	explicit edge_iterator(std::vector<const edge*>::const_iterator);

	// Dereferencable.
	pointer operator*() const;

	// Pre- and post-incrementable.
	edge_iterator& operator++();
	edge_iterator operator++(int);

	// Equality / inequality.
	bool operator==(const edge_iterator& rhs);
	bool operator!=(const edge_iterator& rhs);
};

class edge_iterator_on_multimap
{
private:
	std::multimap<std::size_t, std::shared_ptr<edge>>::const_iterator iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const edge;
	using difference_type = std::ptrdiff_t;
	using pointer = const edge*;
	using reference = const edge&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	edge_iterator_on_multimap() = default;
	explicit edge_iterator_on_multimap(
		std::multimap<std::size_t, std::shared_ptr<edge>>::const_iterator);

	// Dereferencable.
	pointer operator*() const;

	// Pre- and post-incrementable.
	edge_iterator_on_multimap& operator++();
	edge_iterator_on_multimap operator++(int);

	// Equality / inequality.
	bool operator==(const edge_iterator_on_multimap& rhs);
	bool operator!=(const edge_iterator_on_multimap& rhs);
};


template<typename T>
T begin(std::pair<T,T>& iter_pair) { return iter_pair.first; }

template<typename T>
T end(std::pair<T,T>& iter_pair) { return iter_pair.second; }

}
