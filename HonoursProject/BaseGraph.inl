#include <utility>
#include <boost/graph/copy.hpp>
#include <boost/graph/adjacency_list.hpp>

template <class GraphType>
template <class VertexCopier, class EdgeCopier>
BaseGraph<GraphType>::BaseGraph( const BaseGraph<GraphType>& other, VertexCopier vc, EdgeCopier ec )
{
	copy( other, vc, ec );
}

template <class GraphType>
template <class... Args>
BaseGraph<GraphType>::BaseGraph( Args&&... args )
	: _graph( extract(std::forward<Args>(args))... )
{

}

template <class GraphType>
template <class VertexCopier, class EdgeCopier>
void BaseGraph<GraphType>::copy( const BaseGraph<GraphType>& other, VertexCopier vc, EdgeCopier ec )
{
	boost::copy_graph( other._graph, _graph, boost::vertex_copy( vc ).edge_copy( ec ) );
}

template <class GraphType>
BaseGraph<GraphType>::vertex_descriptor BaseGraph<GraphType>::addVertex( vertex_property_type data )
{
	return boost::add_vertex( data, _graph );
}

template <class GraphType>
BaseGraph<GraphType>::edge_descriptor BaseGraph<GraphType>::addEdge( vertex_descriptor source, vertex_descriptor target, edge_property_type data )
{
	return boost::add_edge( source, target, data, _graph ).first;
}

template <class GraphType>
BaseGraph<GraphType>::vertex_descriptor BaseGraph<GraphType>::source( edge_descriptor edge ) const
{
	return boost::source( edge, _graph );
}

template <class GraphType>
BaseGraph<GraphType>::vertex_descriptor BaseGraph<GraphType>::target( edge_descriptor edge ) const
{
	return boost::target( edge, _graph );
}

template <class GraphType>
BaseGraph<GraphType>::vertex_descriptor BaseGraph<GraphType>::other( edge_descriptor edge, vertex_descriptor v ) const
{
	vertex_descriptor s = source( edge );
	return s != v ? s : target( edge );
}

template <class GraphType>
BaseGraph<GraphType>::vertex_property_type& BaseGraph<GraphType>::operator[]( vertex_descriptor v )
{
	return _graph[v];
}

template <class GraphType>
const BaseGraph<GraphType>::vertex_property_type& BaseGraph<GraphType>::operator[]( vertex_descriptor v ) const
{
	return _graph[v];
}

template <class GraphType>
BaseGraph<GraphType>::edge_property_type& BaseGraph<GraphType>::operator[]( edge_descriptor e )
{
	return _graph[e];
}

template <class GraphType>
const BaseGraph<GraphType>::edge_property_type& BaseGraph<GraphType>::operator[]( edge_descriptor e ) const
{
	return _graph[e];
}

template <class GraphType>
BaseGraph<GraphType>::vertices_size_type BaseGraph<GraphType>::numVertices() const
{
	return boost::num_vertices( _graph );
}

template <class GraphType>
BaseGraph<GraphType>::edges_size_type BaseGraph<GraphType>::numEdges() const
{
	return boost::num_edges( _graph );
}

template <class GraphType>
BaseGraph<GraphType>::degree_size_type BaseGraph<GraphType>::degree( vertex_descriptor v ) const
{
	return boost::out_degree( v, _graph );
}


template <class GraphType>
template <class P>
void BaseGraph<GraphType>::vertexMap( P predicate ) const
{
	auto [it, end] = boost::vertices( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType>
template <class P>
void BaseGraph<GraphType>::vertexMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::adjacent_vertices( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType>
template <class P>
void BaseGraph<GraphType>::edgeMap( P predicate ) const
{
	auto [it, end] = boost::edges( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType>
template <class P>
void BaseGraph<GraphType>::edgeMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class GraphType>
template <class T>
T BaseGraph<GraphType>::extract( T arg ) const
{
	return arg;
}

template <class GraphType>
BaseGraph<GraphType>::graph_type& BaseGraph<GraphType>::extract( BaseGraph<GraphType>& g )
{
	return g._graph;
}