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
	return _distance >= other._distance;
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

template<class Graph, template <class> class Result>
inline bool ShortestPaths<Graph, Result>::contains( Vertex v ) const
{
	return _results.contains( v );
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
inline size_t ShortestPaths<Graph, Result>::size() const
{
	return _results.size();
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
		const bool filter = _filter.contains( vertex );
		if (!filter && predicate( result )) { return true; }
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
template <class... Ts>
void DijkstraData<Graph, Result>::decrease( Vertex v, Vertex p, Edge e, double distance, Ts... args )
{
	if (auto search = _handles.find( v ); search != _handles.end())
	{
		Result<Graph>& entry = *(search->second);
		entry.update( p, e, distance, std::forward<Ts>( args )... );
		_queue.update( search->second );
		_distances[v] = distance;
	}
	else
	{
		_handles[v] = _queue.emplace( v, p, e, distance, std::forward<Ts>( args )... );
		_distances[v] = distance;
	}
}

template <class Graph, template<class> class Result>
const Result<Graph>& DijkstraData<Graph, Result>::extract()
{
	const Result<Graph>& top = _queue.top();
	const Vertex v = top.vertex();
	const double dist = top.distance();
	const Result<Graph>& extracted = _results.insert( v, top );
	_queue.pop();
	_handles.erase( v );
	return extracted;
}

template <class Graph, template<class> class Result>
inline bool DijkstraData<Graph, Result>::closed( Vertex v ) const
{
	return _results.contains( v );
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

template <class Graph, template <class> class Result, class P>
ShortestPaths<Graph, Result> dijkstra(
	const Graph&           graph,
	typename Graph::Vertex source,
	P                      predicate
)
{
	DijkstraData<Graph, Result> data( source );

	while (!data.empty())
	{
		const auto ex = data.extract();
		const bool result = graph.edgeMap( ex.vertex(), [&ex, &graph, &predicate, &data]( const auto e ) {
			const auto v = graph.other( e, ex.vertex() );
			if (data.closed( v )) { return false; }
			const double vDist = data.distance( v );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			const PredicateResponse pr = predicate( v, e, newDist );
			if (pr == PredicateResponse::FALSE) { return false; }
			if (pr == PredicateResponse::BREAK) { return true; }
			data.decrease( v, ex.vertex(), e, newDist );
			return false;
		} );
		if (result) { break; }
	}

	return data.results();
}

template<class Graph>
inline bool witnessSearch(
	const Graph&            graph,
	typename Graph::Vertex source,
	typename Graph::Vertex target,
	double                 maxDist
)
{
	bool witnessFound = false;
	const auto test = dijkstra<Graph, DijkstraResult>( graph, source, [&graph, target, maxDist, &witnessFound]( const auto v, const auto e, const double dist ) {
		if (dist > maxDist) { return PredicateResponse::FALSE; }
		if (v == target)
		{
			witnessFound = true;
			return PredicateResponse::BREAK;
		}
		return PredicateResponse::TRUE;
	} );

	return witnessFound;
}

template <class Graph>
ShortestPaths<Graph, DijkstraResult> shortestPaths(
	const Graph&           graph,
	typename Graph::Vertex source,
	double                 maxDist,
	double                 minDist
)
{
	auto paths = dijkstra<Graph, DijkstraResult>( graph, source, [maxDist]( const auto v, const auto e, const double dist ) {
		if (dist > maxDist) { return PredicateResponse::FALSE; }
		return PredicateResponse::TRUE;
	} );

	paths.vertexMap( [minDist, &paths]( const auto& result ) {
		if (result.distance() < minDist)
		{
			paths.filter( result.vertex() );
		}
		return false;
	} );

	return paths;
}

template <class Graph>
bool usefulEdge(
	const Graph&         graph,
	typename Graph::Edge e
)
{
	const auto source = graph.source( e );
	const auto target = graph.target( e );
	const double maxDist = graph[e].weight();

	bool useful = true;
	dijkstra<Graph, DijkstraResult>( graph, source, [&graph, target, maxDist, &useful]( const auto v, const auto e, const double dist ) {
		if (dist >= maxDist) { return PredicateResponse::FALSE; }
		if (v == target)
		{
			useful = false;
			return PredicateResponse::BREAK;
		}
		return PredicateResponse::TRUE;
	} );
	return useful;
}

template <class Graph>
ShortestPaths<Graph, DijkstraResult> dijkstraSearch(
	const Graph&           graph,
	typename Graph::Vertex source,
	typename Graph::Vertex target
)
{
	DijkstraData<Graph, DijkstraResult> data( source );

	while (!data.empty())
	{
		const auto& ex = data.extract();
		if (ex.vertex() == target) { break; }
		graph.edgeMap( ex.vertex(), [&ex, &graph, &data](const auto e) {
			const auto v = graph.other( e, ex.vertex() );
			const double vDist = data.distance( v );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			data.decrease( v, ex.vertex(), e, newDist);
			return false;
		} );
	}

	return data.results();
}