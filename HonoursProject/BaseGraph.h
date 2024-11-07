#ifndef BASEGRAPH_H
#define BASEGRAPH_H

template <class GraphType, class VertexType, class EdgeType>
class MutableGraph;

template <class GraphClass, class VertexPredicate, class EdgePredicate>
class FilteredGraph;

template <class GraphType, class VertexType, class EdgeType>
class BaseGraph
{
private:
	friend MutableGraph<GraphType, VertexType, EdgeType>;

	template <class GraphClass, class VertexPredicate, class EdgePredicate>
	friend class FilteredGraph;

public:
	using graph_type = GraphType;

	using vertex_property_type = typename VertexType;
	using edge_property_type = typename EdgeType;

	using vertex_descriptor = typename graph_type::vertex_descriptor;
	using edge_descriptor = typename graph_type::edge_descriptor;

	using vertices_size_type = typename graph_type::vertices_size_type;
	using edges_size_type = typename graph_type::edges_size_type;
	using degree_size_type = typename graph_type::degree_size_type;

public:
	BaseGraph() = default;
	template <class... Args>
	BaseGraph( int test, Args&&... args );

	template<class GraphType_, class VertexType_, class EdgeType_>
	void copy( const BaseGraph<GraphType_, VertexType_, EdgeType_>& other );

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

public:
	graph_type _graph;
};

#include "BaseGraph.inl"

#endif // BASEGRAPH_H