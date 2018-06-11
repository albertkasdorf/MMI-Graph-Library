#pragma once
#include <iterator>
#include <map>
#include <vector>
#include <memory>
#include <cassert>

namespace graph
{
class vertex;
class edge;

//
// http://anderberg.me/2016/07/04/c-custom-iterators/
//

//------------------------------------------------------------------------------

template<typename V>
class vertex_iterator
{
private:
	std::map<std::size_t, std::shared_ptr<vertex>>::const_iterator _iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const V;
	using difference_type = std::ptrdiff_t;
	using pointer = const V*;
	using reference = const V&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	vertex_iterator() = default;
	explicit vertex_iterator(
		std::map<std::size_t, std::shared_ptr<vertex>>::const_iterator iter)
		:
		_iter(iter)
	{
	}

	// Dereferencable.
	pointer operator*() const
	{
		auto result = dynamic_cast<pointer>((*_iter).second.get());
		assert(result);
		return result;
	}

	// Pre- and post-incrementable.
	vertex_iterator& operator++()
	{
		++_iter;
		return *this;
	}

	vertex_iterator operator++(int)
	{
		vertex_iterator tmp = *this;
		++_iter;
		return tmp;
	}

	// Equality / inequality.
	bool operator==(const vertex_iterator& rhs)
	{
		// Equality: it == end().
		return _iter == rhs._iter;
	}

	bool operator!=(const vertex_iterator& rhs)
	{
		// Inequality: it != end().
		return _iter != rhs._iter;
	}
};


//------------------------------------------------------------------------------

template<typename E>
class edge_iterator
{
private:
	std::vector<const edge*>::const_iterator _iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const E;
	using difference_type = std::ptrdiff_t;
	using pointer = const E*;
	using reference = const E&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	edge_iterator() = default;
	explicit edge_iterator(std::vector<const edge*>::const_iterator iter)
		:
		_iter(iter)
	{
	}

	// Dereferencable.
	pointer operator*() const
	{
		auto result = dynamic_cast<pointer>(*_iter);
		assert(result);
		return result;
	}

	// Pre- and post-incrementable.
	edge_iterator& operator++()
	{
		++_iter;
		return *this;
	}

	edge_iterator operator++(int)
	{
		edge_iterator tmp = *this;
		++_iter;
		return tmp;
	}

	// Equality / inequality.
	bool operator==(const edge_iterator& rhs)
	{
		// Equality: it == end().
		return _iter == rhs._iter;
	}

	bool operator!=(const edge_iterator& rhs)
	{
		// Inequality: it != end().
		return _iter != rhs._iter;
	}
};

//------------------------------------------------------------------------------

template<typename E>
class edge_iterator_on_multimap
{
private:
	std::multimap<std::size_t, std::shared_ptr<edge>>::const_iterator _iter;

public:
	// Iterator traits, previously from std::iterator.
	using value_type = const E;
	using difference_type = std::ptrdiff_t;
	using pointer = const E*;
	using reference = const E&;
	using iterator_category = std::input_iterator_tag;

	// Default constructible.
	edge_iterator_on_multimap() = default;
	explicit edge_iterator_on_multimap(
		std::multimap<std::size_t, std::shared_ptr<edge>>::const_iterator iter)
		:
		_iter(iter)
	{
	}

	// Dereferencable.
	pointer operator*() const
	{
		pointer result = dynamic_cast<pointer>((*_iter).second.get());
		assert(result);
		return result;
	}

	// Pre- and post-incrementable.
	edge_iterator_on_multimap& operator++()
	{
		++_iter;
		return *this;
	}

	edge_iterator_on_multimap operator++(int)
	{
		edge_iterator_on_multimap tmp = *this;
		++_iter;
		return tmp;
	}

	// Equality / inequality.
	bool operator==(const edge_iterator_on_multimap& rhs)
	{
		// Equality: it == end().
		return _iter == rhs._iter;
	}

	bool operator!=(const edge_iterator_on_multimap& rhs)
	{
		// Inequality: it != end().
		return _iter != rhs._iter;
	}
};

//------------------------------------------------------------------------------

template<typename T>
T begin(std::pair<T, T>& iter_pair)
{
	return iter_pair.first;
}

template<typename T>
T end(std::pair<T, T>& iter_pair)
{
	return iter_pair.second;
}

//------------------------------------------------------------------------------

}
