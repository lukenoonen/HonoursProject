#include "Dijkstra.h"
template<class Graph>
inline DijkstraResult<Graph>::DijkstraResult( VertexDescriptor vertex )
	: _vertex( vertex ),
	  _prev( vertex ),
	  _distance( 0.0 )
{

}

template<class Graph>
inline DijkstraResult<Graph>::DijkstraResult(
	VertexDescriptor vertex,
	VertexDescriptor prev,
	EdgeDescriptor   edge,
	double           distance
)
	: _vertex( vertex ),
	  _prev( prev ),
	  _edge( edge ),
	  _distance( distance )
{

}

template<class Graph>
inline void DijkstraResult<Graph>::update( VertexDescriptor prev, EdgeDescriptor edge, double distance )
{
	_prev     = prev;
	_edge     = edge;
	_distance = distance;
}

template<class Graph>
inline bool DijkstraResult<Graph>::operator<( const DijkstraResult<Graph>& other ) const
{
	return _distance < other._distance;
}

template<class Graph>
inline DijkstraResult<Graph>::VertexDescriptor DijkstraResult<Graph>::vertex() const
{
	return _vertex;
}

template<class Graph>
inline DijkstraResult<Graph>::VertexDescriptor DijkstraResult<Graph>::prev() const
{
	return _prev;
}

template<class Graph>
inline DijkstraResult<Graph>::EdgeDescriptor DijkstraResult<Graph>::edge() const
{
	return _edge;
}

template<class Graph>
inline double DijkstraResult<Graph>::distance() const
{
	return _distance;
}

template<class Graph>
inline bool DijkstraResult<Graph>::isOrigin() const
{
	return _vertex == _prev;
}

template <class Graph>
inline ShortestPaths<Graph>::ShortestPaths( VertexDescriptor to )
	: _to( to )
{

}

template <class Graph>
inline void ShortestPaths<Graph>::insert( VertexDescriptor v, const DijkstraResult<Graph>& result )
{
	_results[v] = result;
}

template <class Graph>
inline double ShortestPaths<Graph>::distance( VertexDescriptor from ) const
{
	if (const auto search = _results.find( from ); search != _results.end())
	{
		return search->second.distance();
	}

	return std::numeric_limits<double>::infinity();
}

template<class Graph>
inline void ShortestPaths<Graph>::filter( VertexDescriptor vertex )
{
	_filter.insert( vertex );
}

template <class Graph>
template <class P>
inline bool ShortestPaths<Graph>::vertexMap( P predicate ) const
{
	for (const auto& [vertex, result] : _results)
	{
		if (_filter.contains( vertex )) { continue; }
		if (predicate( result )) { return true; }
	}
	return false;
}

template <class Graph>
template <class P>
inline bool ShortestPaths<Graph>::pathMap( VertexDescriptor from, P predicate ) const
{
	if (!_results.contains( from )) { return false; }
	if (_filter.contains( from )) { return false; }

	VertexDescriptor vertex = from;
	while (true)
	{
		const auto search = _results.find( vertex );
		const DijkstraResult<Graph>& result = search->second;
		if (predicate( result )) { return true; }
		if (vertex == _to) { break; }
		vertex = result.prev();
	}
	return false;
}