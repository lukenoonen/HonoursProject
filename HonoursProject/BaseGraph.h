#ifndef BASEGRAPH_H
#define BASEGRAPH_H

#include <boost/graph/adjacency_list.hpp>

#include <utility>
#include <vector>
#include <unordered_set>
#include <optional>
#include <iostream>

template <class V, class E, template<class, class> class F>
class BaseGraph
{
public:

	using GraphType = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, V, E>;

	using VertexDescriptor = typename GraphType::vertex_descriptor;
	using EdgeDescriptor   = typename GraphType::edge_descriptor;

	struct EdgeHash
	{
		size_t operator() ( const EdgeDescriptor& edge ) const;
	};

	using OptVertexDescriptor = std::optional<typename VertexDescriptor>;
	using OptEdgeDescriptor   = std::optional<typename EdgeDescriptor>;

	using VertexSet    = std::unordered_set<VertexDescriptor>;
	using VertexVector = std::vector<VertexDescriptor>;

	using EdgeSet    = std::unordered_set<EdgeDescriptor, EdgeHash>;
	using EdgeVector = std::vector<EdgeDescriptor>;

	using FilterType = typename F<V, E>;

public:
	BaseGraph();

	VertexDescriptor addVertex( V data );
	OptEdgeDescriptor addEdge( VertexDescriptor source, VertexDescriptor target, E data );

	void removeVertices( const VertexVector& remove );
	void removeIsolatedVertices();

	void removeEdges( const EdgeVector& remove );

	VertexDescriptor source( EdgeDescriptor e ) const;
	VertexDescriptor target( EdgeDescriptor e ) const;
	VertexDescriptor other( EdgeDescriptor e, VertexDescriptor v ) const;

	OptEdgeDescriptor edge( VertexDescriptor source, VertexDescriptor target ) const;

	V& get( VertexDescriptor v );
	const V& get( VertexDescriptor v ) const;
	V& operator[]( VertexDescriptor v );
	const V& operator[]( VertexDescriptor v ) const;

	E& get( EdgeDescriptor e );
	const E& get( EdgeDescriptor e ) const;
	E& operator[]( EdgeDescriptor e );
	const E& operator[]( EdgeDescriptor e ) const;

	size_t numVertices() const;
	size_t numEdges() const;
	size_t degree( VertexDescriptor v ) const;

	double meanDegree() const;
	bool connected() const;

	template <class P>
	bool vertexMap( P predicate ) const;
	template <class P>
	bool vertexMap( VertexDescriptor v, P predicate ) const;

	template <class P>
	bool edgeMap( P predicate ) const;
	template <class P>
	bool edgeMap( VertexDescriptor v, P predicate ) const;

	template <class P>
	bool dfs( VertexDescriptor v, P predicate ) const;

public:
	template <class V_, class E_, template<class, class> class F_>
	friend std::ostream& operator<<( std::ostream& os, const BaseGraph<V_, E_, F_>& graph );

	template <class V_, class E_, template<class, class> class F_>
	friend void serialize( std::ostream& os, const BaseGraph<V_, E_, F_>& data );
	template <class V_, class E_, template<class, class> class F_>
	friend void deserialize( std::istream& is, BaseGraph<V_, E_, F_>& data );

protected:
	GraphType  _graph;
	FilterType _filter;
};

template <class V, class E, template<class, class> class F>
std::ostream& operator<<( std::ostream& os, const BaseGraph<V, E, F>& graph );

template <class V, class E, template<class, class> class F>
void serialize( std::ostream& os, const BaseGraph<V, E, F>& data );
template <class V, class E, template<class, class> class F>
void deserialize( std::istream& is, BaseGraph<V, E, F>& data );

#include "BaseGraph.inl"

#endif // BASEGRAPH_H