#include "Dijkstra.h"

#include <unordered_map>
#include <boost/heap/fibonacci_heap.hpp>

#include <iostream>

namespace
{
	template <class Graph>
	class DijkstraData
	{
	private:
		using VertexDescriptor = typename Graph::VertexDescriptor;
		using EdgeDescriptor   = typename Graph::EdgeDescriptor;

		using Heap        = boost::heap::fibonacci_heap<DijkstraResult<Graph>>;
		using HeapHandle  = DijkstraData<Graph>::Heap::handle_type;
		using HandleMap   = std::unordered_map<VertexDescriptor, HeapHandle>;
		using DistanceMap = std::unordered_map<VertexDescriptor, double>;

	public:
		using ExtractType = std::tuple<VertexDescriptor, double>;

	public:
		DijkstraData( VertexDescriptor to );

		void emplace( VertexDescriptor v, VertexDescriptor p, EdgeDescriptor e, double distance );
		void decrease( VertexDescriptor v, VertexDescriptor p, EdgeDescriptor e, double distance );

		ExtractType extract();

		double distance( VertexDescriptor v ) const;
		bool empty() const;

		ShortestPaths<Graph> results();

	private:
		HandleMap   _handles;
		DistanceMap _distances;

		Heap _queue;

		ShortestPaths<Graph> _results;
	};

	template <class Graph>
	DijkstraData<Graph>::DijkstraData( VertexDescriptor to )
		: _results( to )
	{
		_handles[to]   = _queue.emplace( to );
		_distances[to] = 0.0;
	}

	template <class Graph>
	void DijkstraData<Graph>::emplace( VertexDescriptor v, VertexDescriptor p, EdgeDescriptor e, double distance )
	{
		_handles[v]   = _queue.emplace( v, p, e, distance );
		_distances[v] = distance;
	}

	template <class Graph>
	void DijkstraData<Graph>::decrease( VertexDescriptor v, VertexDescriptor p, EdgeDescriptor e, double distance )
	{
		if (auto search = _handles.find( v ); search != _handles.end())
		{
			DijkstraResult<Graph>& entry = *(search->second);
			entry.update( p, e, distance );
			_queue.decrease( search->second );
			_distances[v] = distance;
		}
		else
		{
			emplace( v, p, e, distance );
		}
	}

	template <class Graph>
	DijkstraData<Graph>::ExtractType DijkstraData<Graph>::extract()
	{
		const DijkstraResult<Graph>& top = _queue.top();
		const VertexDescriptor v = top.vertex();
		const double dist = top.distance();
		_results.insert( v, top );
		_queue.pop();
		_handles.erase( v );
		return { v, dist };
	}

	template <class Graph>
	double DijkstraData<Graph>::distance( VertexDescriptor v ) const
	{
		if (const auto search = _distances.find( v ); search != _distances.end())
		{
			return search->second;
		}
		return std::numeric_limits<double>::infinity();
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

	enum class PredicateResponse : unsigned char
	{
		FALSE,
		TRUE,
		BREAK
	};

	template <class Graph, class P>
	ShortestPaths<Graph> dijkstra(
		const Graph&                     graph,
		typename Graph::VertexDescriptor source,
		P                                predicate
	)
	{
		DijkstraData<Graph> data( source );

		while (!data.empty())
		{
			const auto [u, uDist] = data.extract();
			const bool result = graph.edgeMap( u, [u, uDist, &graph, &predicate, &data]( const auto e ) {
				const auto v = graph.other( e, u );
				const double vDist = data.distance( v );
				const double newDist = uDist + graph[e].weight();
				if (newDist >= vDist) { return false; }
				const PredicateResponse pr = predicate( v, e, newDist );
				if (pr == PredicateResponse::FALSE) { return false; }
				if (pr == PredicateResponse::BREAK) { return true; }
				data.decrease( v, u, e, newDist );
				return false;
			} );
			if (result) { break; }
		}

		return data.results();
	}
}

ShortestPaths<ShortcutGraph> shortestPaths(
	const ShortcutGraph&            graph,
	ShortcutGraph::VertexDescriptor source,
	double                          maxDist,
	double                          minDist,
	double                          maxEdge
)
{
	ShortestPaths<ShortcutGraph> paths = dijkstra( graph, source, [&graph, maxDist]( const auto v, const auto e, const double dist ) {
		if (dist > maxDist) { return PredicateResponse::FALSE; }
		return PredicateResponse::TRUE;
	} );

	paths.vertexMap( [&graph, minDist, maxEdge, &paths]( const auto& result ) {
		if (result.distance() < minDist)
		{
			paths.filter( result.vertex() );
			return false;
		}

		std::vector<ShortcutGraph::VertexDescriptor> path;
		const bool filter = paths.pathMap( result.vertex(), [&graph, maxEdge, &paths, &path]( const auto& result_ ) {
			if (result_.isOrigin()) { return false; }
			path.push_back( result_.vertex() );
			return graph[result_.edge()].maxEdge() > maxEdge;
		} );
		if (filter)
		{
			for (const auto v : path)
			{
				paths.filter( v );
			}
		}
		return false;
	} );

	return paths;
}

bool witnessSearch(
	const ShortcutGraph&            graph,
	ShortcutGraph::VertexDescriptor source,
	ShortcutGraph::VertexDescriptor target,
	ShortcutGraph::VertexDescriptor avoid,
	double                          maxDist
)
{
	bool witnessFound = false;
	dijkstra( graph, source, [&graph, target, avoid, maxDist, &witnessFound]( const auto v, const auto e, const double dist ) {
		if (dist > maxDist) { return PredicateResponse::FALSE; }
		if (v == avoid) { return PredicateResponse::FALSE; }
		if (v == target)
		{
			witnessFound = true;
			return PredicateResponse::BREAK;
		}
		return PredicateResponse::TRUE;
	} );

	return witnessFound;
}

bool usefulEdge(
	const WeightedGraph&          graph,
	WeightedGraph::EdgeDescriptor e
)
{
	const auto source = graph.source( e );
	const auto target = graph.target( e );
	const double maxDist = graph[e].weight();

	bool useful = true;
	dijkstra( graph, source, [&graph, target, maxDist, &useful]( const auto v, const auto e, const double dist ) {
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