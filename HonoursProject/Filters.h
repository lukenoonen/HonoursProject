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
	bool operator()( VertexFilter::Vertex v ) const { return filter.contains( v ); }
	bool operator()( VertexFilter::Edge e ) const { return false; }

	Set<typename VertexFilter::Vertex> filter;
};

#endif // FILTERS_H