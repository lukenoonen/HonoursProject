#ifndef FILTERS_H
#define FILTERS_H

#include "Util.h"

#include "Serialize.h"
#include "BaseGraph.h"

template<class V, class E>
class BaseFilter
{
public:
	using GraphType = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;
	using Vertex    = typename GraphType::vertex_descriptor;
	using Edge      = typename GraphType::edge_descriptor;
};

template<class V, class E>
class NoFilter : public BaseFilter<V, E>
{
public:
	bool operator()( NoFilter::Vertex v ) const { return false; }
	bool operator()( NoFilter::Edge e ) const { return false; }
};

template<class V, class E>
class VertexFilter : public BaseFilter<V, E>
{
public:
	bool operator()( VertexFilter::Vertex v ) const { return _filter.contains( v ); }
	bool operator()( VertexFilter::Edge e ) const { return false; }

	void insert( VertexFilter::Vertex v ) { _filter.insert( v ); }
	void remove( VertexFilter::Vertex v ) { _filter.erase( v ); }
	void clear() { _filter.clear(); }

	size_t size() const { return _filter.size(); }

	const Set<typename VertexFilter::Vertex>& set() const { return _filter; }

private:
	Set<typename VertexFilter::Vertex> _filter;
};

#endif // FILTERS_H