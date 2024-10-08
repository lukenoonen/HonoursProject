#ifndef BASEGRAPH_H
#define BASEGRAPH_H

template <class GraphType>
class BaseGraph
{
public:
	using graph_type = GraphType;

	using vertex_property_type = graph_type::vertex_property_type;
	using edge_property_type = graph_type::edge_property_type;

	using vertex_descriptor = graph_type::vertex_descriptor;
	using edge_descriptor = graph_type::edge_descriptor;

	using vertices_size_type = graph_type::vertices_size_type;
	using edges_size_type = graph_type::edges_size_type;
	using degree_size_type = graph_type::degree_size_type;

public:
	BaseGraph() = default;
	template <class VertexCopier, class EdgeCopier>
	BaseGraph( const BaseGraph<GraphType>& other, VertexCopier vc, EdgeCopier ec );
	template <class... Args>
	BaseGraph( Args&&... args );

	template <class VertexCopier, class EdgeCopier>
	void copy( const BaseGraph<GraphType>& other, VertexCopier vc, EdgeCopier ec );

	vertex_descriptor addVertex( vertex_property_type data );
	edge_descriptor addEdge( vertex_descriptor source, vertex_descriptor target, edge_property_type data );

	vertex_descriptor source( edge_descriptor edge ) const;
	vertex_descriptor target( edge_descriptor edge ) const;
	vertex_descriptor other( edge_descriptor edge, vertex_descriptor v ) const;

	vertex_property_type& operator[]( vertex_descriptor v );
	const vertex_property_type& operator[]( vertex_descriptor v ) const;

	edge_property_type& operator[]( edge_descriptor e );
	const edge_property_type& operator[]( edge_descriptor e ) const;

	vertices_size_type numVertices() const;
	edges_size_type numEdges() const;
	degree_size_type degree( vertex_descriptor v ) const;

	template <class P>
	void vertexMap( P predicate ) const;

	template <class P>
	void vertexMap( vertex_descriptor v, P predicate ) const;

	template <class P>
	void edgeMap( P predicate ) const;

	template <class P>
	void edgeMap( vertex_descriptor v, P predicate ) const;

private:
	template <class T>
	T extract( T arg ) const;
	
	graph_type& extract( BaseGraph<GraphType>& g );

private:
	graph_type _graph;
};

#include "BaseGraph.inl"

#endif // BASEGRAPH_H