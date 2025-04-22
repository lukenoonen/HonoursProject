#ifndef FILTERS_H
#define FILTERS_H

#include "Util.hpp"

#include <boost/graph/adjacency_list.hpp>

template <class V, class E>
class BaseFilter
{
public:
	using GraphType = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;
	using Vertex = typename GraphType::vertex_descriptor;
	using Edge   = typename GraphType::edge_descriptor;
};

template <class V, class E>
class NoFilter : public BaseFilter<V, E>
{
public:
	bool operator()(BaseFilter<V, E>::Vertex /*unused*/) const
	{
		return false;
	}
	bool operator()(BaseFilter<V, E>::Edge /*unused*/) const
	{
		return false;
	}
};

template <class V, class E>
class VertexFilter : public BaseFilter<V, E>
{
public:
	bool operator()(BaseFilter<V, E>::Vertex v) const
	{
		return _filter.contains(v);
	}
	bool operator()(BaseFilter<V, E>::Edge /*unused*/) const
	{
		return false;
	}

	void insert(BaseFilter<V, E>::Vertex v)
	{
		_filter.insert(v);
	}
	void remove(BaseFilter<V, E>::Vertex v)
	{
		_filter.erase(v);
	}
	void clear()
	{
		_filter.clear();
	}

	[[nodiscard]] size_t size() const
	{
		return _filter.size();
	}

	const Set<typename BaseFilter<V, E>::Vertex>& set() const
	{
		return _filter;
	}

private:
	Set<typename BaseFilter<V, E>::Vertex> _filter;
};

#endif  // FILTERS_H