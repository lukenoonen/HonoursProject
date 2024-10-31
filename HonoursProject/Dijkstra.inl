#include <unordered_map>
#include <boost/heap/fibonacci_heap.hpp>

template <class Graph>
class DijkstraData
{
private:
	struct VertexInfo;

private:
	using vertex_descriptor = typename Graph::vertex_descriptor;
	using Heap = boost::heap::fibonacci_heap<DijkstraResult<Graph>>;
	using Map = std::unordered_map<vertex_descriptor, VertexInfo>;

public:
	DijkstraData( vertex_descriptor to );

	void emplace( vertex_descriptor v, double distance );
	vertex_descriptor extract();
	void decrease( vertex_descriptor v, double distance );

	double distance( vertex_descriptor v ) const;
	bool empty() const;

	ShortestPaths<Graph> results();

private:
	Map _info;
	Heap _queue;
	ShortestPaths<Graph> _results;
};

template <class Graph>
DijkstraData<Graph>::DijkstraData( vertex_descriptor to )
	: _results( to )
{

}

template <class Graph>
struct DijkstraData<Graph>::VertexInfo
{
	DijkstraData<Graph>::Heap::handle_type handle;
	double distance;
};

template <class Graph>
void DijkstraData<Graph>::emplace( vertex_descriptor v, double distance )
{
	_info[v] = { _queue.emplace( v, v, distance ), distance };
}

template <class Graph>
DijkstraData<Graph>::vertex_descriptor DijkstraData<Graph>::extract()
{
	const DijkstraResult& top = _queue.top();
	vertex_descriptor v = top.vertex;
	_results.insert( v, top );
	_queue.pop();
	return v;
}

template <class Graph>
void DijkstraData<Graph>::decrease( vertex_descriptor v, double distance )
{
	const auto search = _info.find( v );
	if (const auto search = _info.find( v ); search != _info.end())
	{
		_queue.update( search->second.handle, { v, distance } );
	}
	else
	{
		emplace( v, distance );
	}
}

template <class Graph>
void ShortestPaths<Graph>::insert( vertex_descriptor v, const DijkstraResult<Graph>& result )
{
	_results[v] = result;
}

template <class Graph>
double DijkstraData<Graph>::distance( vertex_descriptor v ) const
{
	return _info[v].distance;
}

template <class Graph>
bool DijkstraData<Graph>::empty() const
{
	return _queue.empty();
}

template <class Graph>
ShortestPaths<Graph> DijkstraData<Graph>::results()
{
	return std::move( _results );
}

template <class Graph>
ShortestPaths<Graph>::ShortestPaths( vertex_descriptor to )
	: _to( to )
{

}

template <class Graph>
double ShortestPaths<Graph>::distance( vertex_descriptor from ) const
{
	if (const auto search = _results.find( from ); search != _results.end())
	{
		return search->second.distance;
	}

	return std::numeric_limits<double>::infinity();
}

template <class Graph>
template <class P>
void ShortestPaths<Graph>::vertexMap( P predicate ) const
{
	for (const auto& [vertex, result] : _results)
	{
		if (predicate( vertex, result.distance )) { break; }
	}
}

template <class Graph>
template <class P>
void ShortestPaths<Graph>::pathMap( vertex_descriptor from, P predicate ) const
{
	if (!_results.contains( from )) { return; }

	vertex_descriptor vertex = from;
	while (true)
	{
		const auto search = _results.find( vertex );
		const DijkstraResult<Graph>& current = search->second;
		if (predicate( current.vertex, current.distance )) { break; }
		if (vertex == _to) { break; }
		vertex = current.prev;
	}
}

template <class Graph>
ShortestPaths<Graph> dijkstraShortestPaths( Graph graph, typename Graph::vertex_descriptor source, double maxDist, double maxEdge )
{
	DijkstraData<Graph> data( source );

	data.emplace( source, 0.0 );

	while (!data.empty())
	{
		const typename Graph::vertex_descriptor u = data.extract();
		const double uDist = data.distance( u );
		graph.edgeMap( u, [u, uDist, maxDist, maxEdge, &graph, &data]( const typename Graph::edge_descriptor e ) {
			const Graph::vertex_descriptor v = graph.other( e, u );
			if (graph[e].maxEdge() > maxEdge) { return false; }
			const double newDist = uDist + graph[e].weight();
			if (newDist > maxDist) { return false; }
			data.decrease( v, newDist );
			return false;
			} );
	}

	return data.results();
}

template <class Graph>
bool dijkstraWitnessSearch( Graph graph, typename Graph::vertex_descriptor source, typename Graph::vertex_descriptor target, typename Graph::vertex_descriptor avoid, double minDist )
{
	DijkstraData<Graph> data;

	data.emplace( source, 0.0 );

	while (!data.empty())
	{
		const typename Graph::vertex_descriptor u = data.extract();
		const double uDist = data.distance( u );
		bool witnessFound = false;
		graph.edgeMap( u, [target, avoid, u, uDist, &graph, &data, &witnessFound]( const typename Graph::edge_descriptor e ) {
			const typename Graph::vertex_descriptor v = graph.other( e, u );
			if (v == avoid) { return false; }
			const double newDist = uDist + graph[e].weight();
			if (v == target && newDist < minDist)
			{
				witnessFound = true;
				return true;
			}
			data.decrease( v, newDist );
			return false;
			} );
		if (witnessFound) { return true; }
	}

	return false;
}