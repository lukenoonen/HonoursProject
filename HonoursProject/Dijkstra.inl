template<class Graph>
inline DijkstraResult<Graph>::DijkstraResult( Vertex vertex )
	: _vertex( vertex ),
	  _prev( vertex ),
	  _distance( 0.0 )
{

}

template<class Graph>
inline DijkstraResult<Graph>::DijkstraResult(
	Vertex vertex,
	Vertex prev,
	Edge   edge,
	double distance
)
	: _vertex( vertex ),
	  _prev( prev ),
	  _edge( edge ),
	  _distance( distance )
{

}

template<class Graph>
inline void DijkstraResult<Graph>::update( Vertex prev, Edge edge, double distance )
{
	_prev     = prev;
	_edge     = edge;
	_distance = distance;
}

template<class Graph>
inline bool DijkstraResult<Graph>::operator<( const DijkstraResult<Graph>& other ) const
{
	return _distance > other._distance;
}

template<class Graph>
inline DijkstraResult<Graph>::Vertex DijkstraResult<Graph>::vertex() const
{
	return _vertex;
}

template<class Graph>
inline DijkstraResult<Graph>::Vertex DijkstraResult<Graph>::prev() const
{
	return _prev;
}

template<class Graph>
inline DijkstraResult<Graph>::Edge DijkstraResult<Graph>::edge() const
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

template <class Graph, template <class> class Result>
inline ShortestPaths<Graph, Result>::ShortestPaths( Vertex to )
	: _to( to )
{

}

template <class Graph, template <class> class Result>
inline const Result<Graph>& ShortestPaths<Graph, Result>::insert( Vertex v, const Result<Graph>& result )
{
	return _results[v] = result;
}

template <class Graph, template <class> class Result>
inline double ShortestPaths<Graph, Result>::distance( Vertex from ) const
{
	if (const auto search = _results.find( from ); search != _results.end())
	{
		return search->second.distance();
	}

	return std::numeric_limits<double>::infinity();
}

template <class Graph, template <class> class Result>
inline void ShortestPaths<Graph, Result>::filter( Vertex vertex )
{
	_filter.insert( vertex );
}

template <class Graph, template <class> class Result>
template <class P>
inline bool ShortestPaths<Graph, Result>::vertexMap( P predicate ) const
{
	for (const auto& [vertex, result] : _results)
	{
		if (_filter.contains( vertex )) { continue; }
		if (predicate( result )) { return true; }
	}
	return false;
}

template <class Graph, template <class> class Result>
template <class P>
inline bool ShortestPaths<Graph, Result>::pathMap( Vertex from, P predicate ) const
{
	if (!_results.contains( from )) { return false; }
	if (_filter.contains( from )) { return false; }

	Vertex vertex = from;
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

template <class Graph, template<class> class Result>
template <class... Ts>
DijkstraData<Graph, Result>::DijkstraData( Vertex to, Ts... args )
	: _results( to )
{
	_handles[to] = _queue.emplace( to, std::forward<Ts>( args )... );
	_distances[to] = 0.0;
}

template <class Graph, template<class> class Result>
void DijkstraData<Graph, Result>::decrease( Vertex v, Vertex p, Edge e, double distance )
{
	if (auto search = _handles.find( v ); search != _handles.end())
	{
		Result<Graph>& entry = *(search->second);
		entry.update( p, e, distance );
		_queue.decrease( search->second );
		_distances[v] = distance;
	}
	else
	{
		_handles[v] = _queue.emplace( v, p, e, distance );
		_distances[v] = distance;
	}
}

template <class Graph, template<class> class Result>
const Result<Graph>& DijkstraData<Graph, Result>::extract()
{
	const DijkstraResult<Graph>& top = _queue.top();
	const Vertex v = top.vertex();
	const double dist = top.distance();
	const DijkstraResult<Graph>& extracted = _results.insert( v, top );
	_queue.pop();
	return extracted;
}

template <class Graph, template<class> class Result>
double DijkstraData<Graph, Result>::distance( Vertex v ) const
{
	if (const auto search = _distances.find( v ); search != _distances.end())
	{
		return search->second;
	}
	return std::numeric_limits<double>::infinity();
}

template <class Graph, template<class> class Result>
bool DijkstraData<Graph, Result>::empty() const
{
	return _queue.empty();
}

template <class Graph, template<class> class Result>
ShortestPaths<Graph, Result> DijkstraData<Graph, Result>::results()
{
	return std::move( _results );
}