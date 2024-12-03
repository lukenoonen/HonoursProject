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
		return search->second.distance;
	}

	return std::numeric_limits<double>::infinity();
}

template <class Graph>
template <class P>
inline void ShortestPaths<Graph>::vertexMap( P predicate ) const
{
	for (const auto& [vertex, result] : _results)
	{
		if (predicate( vertex, result.distance )) { break; }
	}
}

template <class Graph>
template <class P>
inline void ShortestPaths<Graph>::pathMap( VertexDescriptor from, P predicate ) const
{
	if (!_results.contains( from )) { return; }

	VertexDescriptor vertex = from;
	while (true)
	{
		const auto search = _results.find( vertex );
		const DijkstraResult<Graph>& current = search->second;
		if (predicate( current.vertex, current.distance )) { break; }
		if (vertex == _to) { break; }
		vertex = current.prev;
	}
}