#ifndef FILTERS_H
#define FILTERS_H

#include "BaseGraph.h"

template<class V, class E>
class BaseFilter
{
public:
	using GraphType = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;
	using VertexDescriptor = typename GraphType::vertex_descriptor;
	using EdgeDescriptor = typename GraphType::edge_descriptor;
};

template<class V, class E>
class NoFilter : public BaseFilter<V, E>
{
public:
	bool operator()( NoFilter::VertexDescriptor v ) const { return false; }
	bool operator()( NoFilter::EdgeDescriptor e ) const { return false; }
};

template<class V, class E>
class VertexFilter : public BaseFilter<V, E>
{
public:
	bool operator()( VertexFilter::VertexDescriptor v ) const { return filter.contains( v ); }
	bool operator()( VertexFilter::EdgeDescriptor e ) const { return false; }

	std::unordered_set<typename VertexFilter::VertexDescriptor> filter;
};

#endif // FILTERS_H