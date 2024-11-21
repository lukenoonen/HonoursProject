#include <unordered_map>
#include <boost/heap/fibonacci_heap.hpp>

template <class Graph>
class DijkstraData
{
private:
	struct VertexInfo;

private:
	using VertexDescriptor = typename Graph::VertexDescriptor;
	using Heap = boost::heap::fibonacci_heap<DijkstraResult<Graph>>;
	using Map = std::unordered_map<VertexDescriptor, VertexInfo>;

public:
	DijkstraData( VertexDescriptor to );

	void emplace( VertexDescriptor v, VertexDescriptor p, double distance );
	VertexDescriptor extract();
	void decrease( VertexDescriptor v, VertexDescriptor p, double distance );

	double distance( VertexDescriptor v ) const;
	bool empty() const;

	ShortestPaths<Graph> results();

private:
	Map _info;
	Heap _queue;
	ShortestPaths<Graph> _results;
};

template <class Graph>
DijkstraData<Graph>::DijkstraData( VertexDescriptor to )
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
void DijkstraData<Graph>::emplace( VertexDescriptor v, VertexDescriptor p,  double distance )
{
	_info[v] = { _queue.emplace( v, p, distance ), distance };
}

template <class Graph>
DijkstraData<Graph>::VertexDescriptor DijkstraData<Graph>::extract()
{
	const DijkstraResult<Graph>& top = _queue.top();
	VertexDescriptor v = top.vertex;
	_results.insert( v, top );
	_queue.pop();
	return v;
}

template <class Graph>
void DijkstraData<Graph>::decrease( VertexDescriptor v, VertexDescriptor p, double distance )
{
	if (const auto search = _info.find( v ); search != _info.end())
	{
		_queue.update( search->second.handle, { v, p, distance } );
	}
	else
	{
		emplace( v, p, distance );
	}
}

template <class Graph>
void ShortestPaths<Graph>::insert( VertexDescriptor v, const DijkstraResult<Graph>& result )
{
	_results[v] = result;
}

template <class Graph>
double DijkstraData<Graph>::distance( VertexDescriptor v ) const
{
	if (const auto search = _info.find( v ); search != _info.end())
	{
		return search->second.distance;
	}
	return -1.0;
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
ShortestPaths<Graph>::ShortestPaths( VertexDescriptor to )
	: _to( to )
{

}

template <class Graph>
double ShortestPaths<Graph>::distance( VertexDescriptor from ) const
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
void ShortestPaths<Graph>::pathMap( VertexDescriptor from, P predicate ) const
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

template <class Graph>
ShortestPaths<Graph> dijkstraShortestPaths( Graph graph, typename Graph::VertexDescriptor source, double maxDist, double maxEdge )
{
	DijkstraData<Graph> data( source );

	data.emplace( source, source, 0.0 );

	while (!data.empty())
	{
		const typename Graph::VertexDescriptor u = data.extract();
		const double uDist = data.distance( u );
		graph.edgeMap( u, [u, uDist, maxDist, maxEdge, &graph, &data]( const typename Graph::edge_descriptor e ) {
			const Graph::VertexDescriptor v = graph.other( e, u );
			if (graph[e].maxEdge() > maxEdge) { return false; }
			const double newDist = uDist + graph[e].weight();
			if (newDist > maxDist) { return false; }
			data.decrease( v, u, newDist );
			return false;
			} );
	}

	return data.results();
}

template <class Graph>
bool dijkstraWitnessSearch( Graph graph, typename Graph::VertexDescriptor source, typename Graph::VertexDescriptor target, typename Graph::VertexDescriptor avoid, double minDist )
{
	DijkstraData<Graph> data( source );

	data.emplace( source, source, 0.0 );

	while (!data.empty())
	{
		const typename Graph::VertexDescriptor u = data.extract();
		const double uDist = data.distance( u );
		bool witnessFound = false;
		graph.edgeMap( u, [target, avoid, u, uDist, minDist, &graph, &data, &witnessFound]( const typename Graph::edge_descriptor e ) {
			const typename Graph::VertexDescriptor v = graph.other( e, u );
			if (v == avoid) { return false; }
			const double newDist = uDist + graph[e].weight();
			if (v == target && newDist < minDist)
			{
				witnessFound = true;
				return true;
			}
			data.decrease( v, u, newDist );
			return false;
			} );
		if (witnessFound) { return true; }
	}

	return false;
}