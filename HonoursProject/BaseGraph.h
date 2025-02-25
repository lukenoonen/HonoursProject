#ifndef BASEGRAPH_H
#define BASEGRAPH_H

#include "Util.h"

#include <boost/graph/adjacency_list.hpp>

#include <utility>
#include <vector>
#include <unordered_set>
#include <optional>
#include <iostream>

template <class Edge>
struct EdgeHash
{
	size_t operator() ( const Edge& edge ) const;
};

template <class Edge>
struct EdgeCompare
{
	bool operator() ( const Edge& a, const Edge& b ) const
	{
		return (a.m_source == b.m_source && a.m_target == b.m_target)
			|| (a.m_source == b.m_target && a.m_target == b.m_source);
	}
};

template<class Edge>
using EdgeSet = Set<Edge, EdgeHash<Edge>, EdgeCompare<Edge>>;

template <class V, class E, template<class, class> class F>
class BaseGraph
{
public:
	using GraphType = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;

	using Vertex = typename GraphType::vertex_descriptor;
	using Edge   = typename GraphType::edge_descriptor;

	using FilterType = typename F<V, E>;

public:
	BaseGraph();

	Vertex addVertex( V data );
	Opt<Edge> addEdge( Vertex s, Vertex t, E data );

	void removeVertices( const Set<Vertex>& remove );
	void removeIsolatedVertices();

	void removeEdges( const EdgeSet<Edge>& remove );

	Vertex source( Edge e ) const;
	Vertex target( Edge e ) const;
	Vertex other( Edge e, Vertex v ) const;

	Opt<Edge> edge( Vertex s, Vertex t ) const;

	V& get( Vertex v );
	const V& get( Vertex v ) const;
	V& operator[]( Vertex v );
	const V& operator[]( Vertex v ) const;

	E& get( Edge e );
	const E& get( Edge e ) const;
	E& operator[]( Edge e );
	const E& operator[]( Edge e ) const;

	size_t numVertices() const;
	size_t numEdges() const;
	size_t degree( Vertex v ) const;

	double meanDegree() const;
	bool connected() const;

	template <class P>
	bool vertexMap( P predicate ) const;
	template <class P>
	bool vertexMap( Vertex v, P predicate ) const;

	template <class P>
	bool edgeMap( P predicate ) const;
	template <class P>
	bool edgeMap( Vertex v, P predicate ) const;
	template <class P>
	bool neighbourEdgeMap( Vertex v, P predicate ) const;

	template <class P>
	bool dfs( Vertex v, P predicate ) const;

public:
	template <class V_, class E_, template<class, class> class F_>
	friend std::ostream& operator<<( std::ostream& os, const BaseGraph<V_, E_, F_>& graph );

	template <class V_, class E_, template<class, class> class F_>
	friend void serialize( std::ostream& os, const BaseGraph<V_, E_, F_>& data );
	template <class V_, class E_, template<class, class> class F_>
	friend void deserialize( std::istream& is, BaseGraph<V_, E_, F_>& data );

public:
	GraphType  _graph;
	FilterType _filter;
};

#include "BaseGraph.inl"

#endif // BASEGRAPH_H