#ifndef GRAPH_H
#define GRAPH_H

#include <boost/graph/adjacency_list.hpp>

template <class V, class E>
class Graph
{
public:
	using graph_type = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;

	using vertex_descriptor = graph_type::vertex_descriptor;
	using edge_descriptor = graph_type::edge_descriptor;

	using vertices_size_type = graph_type::vertices_size_type;
	using edges_size_type = graph_type::edges_size_type;
	using degree_size_type = graph_type::degree_size_type;

public:
	Graph() = default;

	vertex_descriptor addVertex( V data );
	edge_descriptor addEdge( vertex_descriptor source, vertex_descriptor target, E data );

	vertex_descriptor source( edge_descriptor edge ) const;
	vertex_descriptor target( edge_descriptor edge ) const;

	V& operator[]( vertex_descriptor v );
	const V& operator[]( vertex_descriptor v ) const;

	E& operator[]( edge_descriptor e );
	const E& operator[]( edge_descriptor e ) const;

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
	graph_type _graph;
};

#include "Graph.inl"

#endif // GRAPH_H