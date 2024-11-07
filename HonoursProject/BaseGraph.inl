#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/copy.hpp>

template <class GraphType, class VertexType, class EdgeType>
template <class... Args>
BaseGraph<GraphType, VertexType, EdgeType>::BaseGraph( int test, Args&&... args )
	: _graph( std::forward<Args>( args )... )
{

}

template <class GraphType, class VertexType, class EdgeType>
template<class GraphType_, class VertexType_, class EdgeType_>
void BaseGraph<GraphType, VertexType, EdgeType>::copy( const BaseGraph<GraphType_, VertexType_, EdgeType_>& other )
{
	boost::copy_graph( other._graph, _graph );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::vertex_descriptor BaseGraph<GraphType, VertexType, EdgeType>::source( edge_descriptor edge ) const
{
	return boost::source( edge, _graph );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::vertex_descriptor BaseGraph<GraphType, VertexType, EdgeType>::target( edge_descriptor edge ) const
{
	return boost::target( edge, _graph );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::vertex_descriptor BaseGraph<GraphType, VertexType, EdgeType>::other( edge_descriptor edge, vertex_descriptor v ) const
{
	vertex_descriptor s = source( edge );
	return s != v ? s : target( edge );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::vertex_property_type& BaseGraph<GraphType, VertexType, EdgeType>::operator[]( vertex_descriptor v )
{
	return _graph[v];
}

template <class GraphType, class VertexType, class EdgeType>
const BaseGraph<GraphType, VertexType, EdgeType>::vertex_property_type& BaseGraph<GraphType, VertexType, EdgeType>::operator[]( vertex_descriptor v ) const
{
	return _graph[v];
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::edge_property_type& BaseGraph<GraphType, VertexType, EdgeType>::operator[]( edge_descriptor e )
{
	return _graph[e];
}

template <class GraphType, class VertexType, class EdgeType>
const BaseGraph<GraphType, VertexType, EdgeType>::edge_property_type& BaseGraph<GraphType, VertexType, EdgeType>::operator[]( edge_descriptor e ) const
{
	return _graph[e];
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::vertices_size_type BaseGraph<GraphType, VertexType, EdgeType>::numVertices() const
{
	return boost::num_vertices( _graph );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::edges_size_type BaseGraph<GraphType, VertexType, EdgeType>::numEdges() const
{
	return boost::num_edges( _graph );
}

template <class GraphType, class VertexType, class EdgeType>
BaseGraph<GraphType, VertexType, EdgeType>::degree_size_type BaseGraph<GraphType, VertexType, EdgeType>::degree( vertex_descriptor v ) const
{
	return boost::out_degree( v, _graph );
}


template <class GraphType, class VertexType, class EdgeType>
template <class P>
void BaseGraph<GraphType, VertexType, EdgeType>::vertexMap( P predicate ) const
{
	auto [it, end] = boost::vertices( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType, class VertexType, class EdgeType>
template <class P>
void BaseGraph<GraphType, VertexType, EdgeType>::vertexMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::adjacent_vertices( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType, class VertexType, class EdgeType>
template <class P>
void BaseGraph<GraphType, VertexType, EdgeType>::edgeMap( P predicate ) const
{
	auto [it, end] = boost::edges( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType, class VertexType, class EdgeType>
template <class P>
void BaseGraph<GraphType, VertexType, EdgeType>::edgeMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}