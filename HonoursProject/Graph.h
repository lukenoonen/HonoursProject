#ifndef GRAPH_H
#define GRAPH_H

#include "BaseGraph.h"

#include <boost/graph/adjacency_list.hpp>

template <class VertexType, class EdgeType>
class Graph : public BaseGraph<boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexType, EdgeType>>
{
public:
	Graph() = default;
	template <class VertexCopier, class EdgeCopier>
	Graph( const Graph<VertexType, EdgeType>& other, VertexCopier vc, EdgeCopier ec );
};

#include "Graph.inl"

#endif // GRAPH_H