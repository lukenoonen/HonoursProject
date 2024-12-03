#include "Serialize.h"

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <stack>

template <class V, class E, template<class, class> class F>
inline size_t BaseGraph<V, E, F>::EdgeHash::operator()( const EdgeDescriptor& edge ) const
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
inline BaseGraph<V, E, F>::VertexDescriptor BaseGraph<V, E, F>::addVertex( V data )
{
	return boost::add_vertex( std::move( data ), _graph );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::OptEdgeDescriptor BaseGraph<V, E, F>::addEdge( VertexDescriptor source, VertexDescriptor target, E data )
{
	if (_filter( source ) || _filter( target )) { return {}; }
	auto result = boost::add_edge( source, target, std::move( data ), _graph );
	if (!result.second) { return {}; }
	return result.first;
}

template <class V, class E, template<class, class> class F>
inline void BaseGraph<V, E, F>::removeVertices( const VertexVector& remove )
{
	if (remove.empty()) { return; }

	const VertexSet removeSet( remove.begin(), remove.end() );
	std::unordered_map<VertexDescriptor, VertexDescriptor> toUpdated;
	GraphType updated;

	vertexMap( [this, &removeSet, &toUpdated, &updated]( const auto v ) {
		if (removeSet.contains( v )) { return false; }
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
	VertexVector remove;
	vertexMap( [this, &remove]( const auto v ) {
		if (degree( v ) == 0)
		{
			remove.push_back( v );
		}
		return false;
	} );
	removeVertices( remove );
}

template <class V, class E, template<class, class> class F>
void BaseGraph<V, E, F>::removeEdges( const EdgeVector& remove )
{
	if (remove.empty()) { return; }

	const EdgeSet removeSet( remove.begin(), remove.end() );
	GraphType updated;

	vertexMap( [this, &updated]( const auto v ) {
		boost::add_vertex( std::move( get( v ) ), updated );
		return false;
	} );

	edgeMap( [this, &removeSet, &updated]( const auto e ) {
		if (removeSet.contains( e )) { return false; }
		boost::add_edge( source( e ), target( e ), std::move( get( e ) ), updated );
		return false;
	} );

	_graph = std::move( updated );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::VertexDescriptor BaseGraph<V, E, F>::source( EdgeDescriptor e ) const
{
	return boost::source( e, _graph );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::VertexDescriptor BaseGraph<V, E, F>::target( EdgeDescriptor e ) const
{
	return boost::target( e, _graph );
}

template <class V, class E, template<class, class> class F>
BaseGraph<V, E, F>::VertexDescriptor BaseGraph<V, E, F>::other( EdgeDescriptor e, VertexDescriptor v ) const
{
	const VertexDescriptor s = source( e );
	return s != v ? s : target( e );
}

template <class V, class E, template<class, class> class F>
inline BaseGraph<V, E, F>::OptEdgeDescriptor BaseGraph<V, E, F>::edge( VertexDescriptor source, VertexDescriptor target ) const
{
	auto result = boost::edge( source, target, _graph );
	if (!result.second) { return {}; }
	return result.first;
}

template <class V, class E, template<class, class> class F>
inline V& BaseGraph<V, E, F>::get( VertexDescriptor v )
{
	return _graph[v];
}

template <class V, class E, template<class, class> class F>
inline const V& BaseGraph<V, E, F>::get( VertexDescriptor v ) const
{
	return _graph[v];
}

template <class V, class E, template<class, class> class F>
inline V& BaseGraph<V, E, F>::operator[]( VertexDescriptor v )
{
	return get( v );
}

template <class V, class E, template<class, class> class F>
inline const V& BaseGraph<V, E, F>::operator[]( VertexDescriptor v ) const
{
	return get( v );
}

template <class V, class E, template<class, class> class F>
inline E& BaseGraph<V, E, F>::get( EdgeDescriptor e )
{
	return _graph[e];
}

template <class V, class E, template<class, class> class F>
inline const E& BaseGraph<V, E, F>::get( EdgeDescriptor e ) const
{
	return _graph[e];
}

template <class V, class E, template<class, class> class F>
inline E& BaseGraph<V, E, F>::operator[]( EdgeDescriptor e )
{
	return get( e );
}

template <class V, class E, template<class, class> class F>
inline const E& BaseGraph<V, E, F>::operator[]( EdgeDescriptor e ) const
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
inline size_t BaseGraph<V, E, F>::degree( VertexDescriptor v ) const
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
template <class P>
inline bool BaseGraph<V, E, F>::vertexMap( P predicate ) const
{
	auto [it, end] = boost::vertices( _graph );
	while (it != end)
	{
		const auto v = *it;
		if (!_filter( v ) && predicate( v )) { return true; }
		it++;
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::vertexMap( VertexDescriptor v, P predicate ) const
{
	if (_filter( v )) { return false; }
	auto [it, end] = boost::adjacent_vertices( v, _graph );
	while (it != end)
	{
		const auto u = *it;
		if (!_filter( u ) && predicate( u )) { return true; }
		it++;
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
		const auto e = *it;
		const auto u = source( e );
		const auto v = target( e );
		const bool filter = _filter( e ) || _filter( u ) || _filter( v );
		if (!filter && predicate( e )) { return true; }
		it++;
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template <class P>
inline bool BaseGraph<V, E, F>::edgeMap( VertexDescriptor v, P predicate ) const
{
	if (_filter( v )) { return false; }
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		const auto e = *it;
		const auto u = other( e, v );
		const bool filter = _filter( e );
		const bool filter2 = _filter( u );
		if (!filter && !filter2 && predicate( e )) { return true; }
		it++;
	}
	return false;
}

template <class V, class E, template<class, class> class F>
template<class P>
inline bool BaseGraph<V, E, F>::dfs( VertexDescriptor source, P predicate ) const
{
	if (_filter( source )) { return false; }
	std::vector<bool> visited( numVertices(), false );
	std::stack<VertexDescriptor> stack;
	visited[source] = true;
	stack.push( source );
	while (!stack.empty())
	{
		const auto v = stack.top();
		stack.pop();
		if (predicate( v )) { return true; }
		vertexMap( v, [this, &visited, &stack]( const auto u ) {
			if (visited[u]) { return false; }
			visited[u] = true;
			stack.push( u );
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
}

template <class V, class E, template<class, class> class F>
inline void deserialize( std::istream& is, BaseGraph<V, E, F>& data )
{
	using VertexDescriptor = BaseGraph<V, E, F>::VertexDescriptor;

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
		VertexDescriptor s, t;
		deserialize( is, s );
		deserialize( is, t );
		E edge;
		deserialize( is, edge );
		data.addEdge( s, t, std::move( edge ) );
	}
}