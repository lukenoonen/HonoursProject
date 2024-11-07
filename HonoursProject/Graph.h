#ifndef GRAPH_H
#define GRAPH_H

#include "MutableGraph.h"

#include <boost/graph/adjacency_list.hpp>

template <class VertexType, class EdgeType>
class Graph : public MutableGraph<boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexType, EdgeType>, VertexType, EdgeType>
{
public:
	Graph() = default;
};

#include "Graph.inl"

#endif // GRAPH_H