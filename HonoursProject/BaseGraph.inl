#include "Serialize.h"

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <stack>

template <class Edge>
inline size_t EdgeHash<Edge>::operator()( const Edge& edge ) const
{
	const auto u = std::max( edge.m_source, edge.m_target );
	const auto v = std::min( edge.m_source, edge.m_target );
	size_t hash = 0;
	boost::hash_combine( hash, u );
	boost::hash_combine( hash, v );
	return hash;
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::BaseGraph()
{

}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::Vertex BaseGraph<V, E, F>::addVertex( V data )
{
	return boost::add_vertex( std::move( data ), _graph );
}

template <class V, class E, template<class, class> class F>
inline Opt<typename BaseGraph<V, E, F>::Edge> BaseGraph<V, E, F>::addEdge( Vertex s, Vertex t, E data )
{
	if (_filter( s ) || _filter( t )) { return {}; }
	auto result = boost::add_edge( s, t, std::move( data ), _graph );
	if (!result.second) { return {}; }
	return result.first;
}

template <class V, class E, template<class, class> class F>
inline void BaseGraph<V, E, F>::removeVertices( const Set<Vertex>& remove )
{
	if (remove.empty()) { return; }

	Map<Vertex, Vertex> toUpdated;
	GraphType updated;

	vertexMap( [this, &remove, &toUpdated, &updated]( const auto v ) {
		if (remove.contains( v )) { return false; }
		toUpdated[v] = boost::add_vertex( std::move( get( v ) ), updated );
		return false;
	} );

	edgeMap( [this, &toUpdated, &updated]( const auto e ) {
		auto uSearch = toUpdated.find( source( e ) );
		if (uSearch == toUpdated.end()) { return false; }
		auto vSearch = toUpdated.find( target( e ) );
		if (vSearch == toUpdated.end()) { return false; }
		boost::add_edge( uSearch->second, vSearch->second, std::move( get( e ) ), updated );
		return false;
	} );

	_graph = std::move( updated );
}

template <class V, class E, template<class, class> class F>
inline void BaseGraph<V, E, F>::removeIsolatedVertices()
{
	Set<Vertex> remove;
	vertexMap( [this, &remove]( const auto v ) {
		if (degree( v ) == 0)
		{
			remove.insert( v );
		}
		return false;
	} );
	removeVertices( remove );
}

template <class V, class E, template<class, class> class F>
void BaseGraph<V, E, F>::removeEdges( const EdgeSet<Edge>& remove )
{
	if (remove.empty()) { return; }

	GraphType updated;

	vertexMap( [this, &updated]( const auto v ) {
		boost::add_vertex( std::move( get( v ) ), updated );
		return false;
	} );

	edgeMap( [this, &remove, &updated]( const auto e ) {
		if (remove.contains( e )) { return false; }
		boost::add_edge( source( e ), target( e ), std::move( get( e ) ), updated );
		return false;
	} );

	_graph = std::move( updated );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::Vertex BaseGraph<V, E, F>::source( Edge e ) const
{
	return boost::source( e, _graph );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::Vertex BaseGraph<V, E, F>::target( Edge e ) const
{
	return boost::target( e, _graph );
}

template <class V, class E, template<class, class> class F>
BaseGraph<V, E, F>::Vertex BaseGraph<V, E, F>::other( Edge e, Vertex v ) const
{
	const Vertex u = source( e );
	return u != v ? u : target( e );
}

template <class V, class E, template<class, class> class F>
inline Opt<typename BaseGraph<V, E, F>::Edge> BaseGraph<V, E, F>::edge( Vertex s, Vertex t ) const
{
	auto result = boost::edge( s, t, _graph );
	if (!result.second) { return {}; }
	return result.first;
}

template <class V, class E, template<class, class> class F>
inline V& BaseGraph<V, E, F>::get( Vertex v )
{
	return _graph[v];
}

template <class V, class E, template<class, class> class F>
inline const V& BaseGraph<V, E, F>::get( Vertex v ) const
{
	return _graph[v];
}

template <class V, class E, template<class, class> class F>
inline V& BaseGraph<V, E, F>::operator[]( Vertex v )
{
	return get( v );
}

template <class V, class E, template<class, class> class F>
inline const V& BaseGraph<V, E, F>::operator[]( Vertex v ) const
{
	return get( v );
}

template <class V, class E, template<class, class> class F>
inline E& BaseGraph<V, E, F>::get( Edge e )
{
	return _graph[e];
}

template <class V, class E, template<class, class> class F>
inline const E& BaseGraph<V, E, F>::get( Edge e ) const
{
	return _graph[e];
}

template <class V, class E, template<class, class> class F>
inline E& BaseGraph<V, E, F>::operator[]( Edge e )
{
	return get( e );
}

template <class V, class E, template<class, class> class F>
inline const E& BaseGraph<V, E, F>::operator[]( Edge e ) const
{
	return get( e );
}

template <class V, class E, template<class, class> class F>
inline size_t BaseGraph<V, E, F>::numVertices() const
{
	return boost::num_vertices( _graph );
}

template <class V, class E, template<class, class> class F>
inline size_t BaseGraph<V, E, F>::numEdges() const
{
	return boost::num_edges( _graph );
}

template <class V, class E, template<class, class> class F>
inline size_t BaseGraph<V, E, F>::degree( Vertex v ) const
{
	return boost::degree( v, _graph );
}

template <class V, class E, template<class, class> class F>
inline double BaseGraph<V, E, F>::meanDegree() const
{
	const double e = static_cast<double>(numEdges());
	const double n = static_cast<double>(numVertices());
	return 2.0 * e / n;
}

template <class V, class E, template<class, class> class F>
inline bool BaseGraph<V, E, F>::connected() const
{
	size_t count = 0;
	dfs( 0, [&count]( const auto v ) {
		count++;
		return false;
	} );
	return count == numVertices();
}

template <class V, class E, template<class, class> class F>
inline Set<typename BaseGraph<V, E, F>::Vertex> BaseGraph<V, E, F>::connectedComponent( Vertex v ) const
{
	Set<Vertex> result;
	dfs( v, [&result]( const auto v_ ) {
		result.insert( v_ );
		return false;
	} );
	return result;
}

template <class V, class E, template<class, class> class F>
inline Vec<Set<typename BaseGraph<V, E, F>::Vertex>> BaseGraph<V, E, F>::connectedComponents() const
{
	Vec<Set<Vertex>> result;
	vertexMap( [this, &result]( const auto v ) {
		for (const auto& cc : result)
		{
			if (cc.contains( v )) { return false; }
		}
		result.emplace_back( connectedComponent( v ) );
		return false;
	} );
	return result;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::vertexMap( P predicate ) const
{
	auto [it, end] = boost::vertices( _graph );
	while (it != end)
	{
		const auto v = *it++;
		if (!_filter( v ) && predicate( v )) { return true; }
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::vertexMap( Vertex v, P predicate ) const
{
	if (_filter( v )) { return false; }
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		const auto e = *it++;
		const auto u = other( e, v );
		const bool filter = _filter( e ) || _filter( u );
		if (!filter && predicate( u )) { return true; }
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::edgeMap( P predicate ) const
{
	auto [it, end] = boost::edges( _graph );
	while (it != end)
	{
		const auto e = *it++;
		if (_filter( e ) || _filter( source( e ) ) || _filter( target( e ) )) { continue; }
		if (predicate( e )) { return true; }
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::edgeMap( Vertex v, P predicate ) const
{
	if (_filter( v )) { return false; }
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		const auto e = *it++;
		if (_filter( e ) || _filter( other( e, v ) )) { continue; }
		if (predicate( e )) { return true; }
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::neighbourEdgeMap( Vertex v, P predicate ) const
{
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		const auto eu = *it++;
		if (_filter( eu ) || _filter( other( eu, v ) )) { continue; }

		auto itt = it;
		while (itt != end)
		{
			const auto ew = *itt++;
			if (_filter( ew ) || _filter( other( ew, v ) )) { continue; }
			if (predicate( eu, ew )) { return true; }
		}
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template<class P>
inline bool BaseGraph<V, E, F>::dfs( Vertex v, P predicate ) const
{
	if (_filter( v )) { return false; }
	Vec<bool> visited( numVertices(), false );
	std::stack<Vertex> stack;
	visited[v] = true;
	stack.push( v );
	while (!stack.empty())
	{
		const auto u = stack.top();
		stack.pop();
		if (predicate( u )) { return true; }
		vertexMap( u, [this, &visited, &stack]( const auto w ) {
			if (visited[w]) { return false; }
			visited[w] = true;
			stack.push( w );
			return false;
		} );
	}
	return false;
}

template <class V, class E, template<class, class> class F>
inline std::ostream& operator<<( std::ostream& os, const BaseGraph<V, E, F>& graph )
{
	os << "(|V|, |E|) = (" << graph.numVertices() << ", " << graph.numEdges() << ")\n";
	graph.vertexMap( [&graph, &os]( const auto v ) {
		os << v << " ->";
		if (graph.degree( v ) == 0)
		{
			os << "n/a";
		}
		else
		{
			graph.vertexMap( v, [&graph, &os]( const auto u ) {
				os << ' ' << u;
				return false;
			} );
		}
		return false;
	} );
	return os;
}

template <class V, class E, template<class, class> class F>
inline void serialize( std::ostream& os, const BaseGraph<V, E, F>& data )
{
	serialize( os, data.numVertices() );
	data.vertexMap( [&data, &os]( const auto v ) {
		serialize( os, data[v] );
		return false;
	} );

	serialize( os, data.numEdges() );
	data.edgeMap( [&data, &os]( const auto e ) {
		serialize( os, data.source( e ) );
		serialize( os, data.target( e ) );
		serialize( os, data[e] );
		return false;
	} );

	// serialize( os, data._filter );
}

template <class V, class E, template<class, class> class F>
inline void deserialize( std::istream& is, BaseGraph<V, E, F>& data )
{
	using Vertex = BaseGraph<V, E, F>::Vertex;

	size_t numVertices;
	deserialize( is, numVertices );
	for (size_t i = 0; i < numVertices; i++)
	{
		V vertex;
		deserialize( is, vertex );
		data.addVertex( std::move( vertex ) );
	}

	size_t numEdges;
	deserialize( is, numEdges );
	for (size_t i = 0; i < numEdges; i++)
	{
		Vertex s, t;
		deserialize( is, s );
		deserialize( is, t );
		E edge;
		deserialize( is, edge );
		data.addEdge( s, t, std::move( edge ) );
	}

	// deserialize( is, data._filter );
}