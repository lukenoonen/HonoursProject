#ifndef MUTABLEGRAPH_H
#define MUTABLEGRAPH_H

#include "BaseGraph.h"

template <class GraphType, class VertexType, class EdgeType>
class MutableGraph : public BaseGraph<GraphType, VertexType, EdgeType>
{
private:
	using BaseClass = BaseGraph<GraphType, VertexType, EdgeType>;

public:
	MutableGraph() = default;

	BaseClass::vertex_descriptor addVertex( BaseClass::vertex_property_type data )
	{
		return boost::add_vertex( data, BaseClass::_graph );
	}

	BaseClass::edge_descriptor addEdge( BaseClass::vertex_descriptor source, BaseClass::vertex_descriptor target, BaseClass::edge_property_type data )
	{
		return boost::add_edge( source, target, data, BaseClass::_graph ).first;
	}
};

#include "MutableGraph.inl"

#endif // MUTABLEGRAPH_H