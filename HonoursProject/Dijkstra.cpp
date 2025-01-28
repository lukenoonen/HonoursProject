#include "Dijkstra.h"

namespace
{
	enum class PredicateResponse : unsigned char
	{
		FALSE,
		TRUE,
		BREAK
	};

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
				const double vDist   = data.distance( v );
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
}

CREATE_GLOBAL_PROFILER( total, shortest_paths );
CREATE_LOCAL_PROFILER( dijkstra, shortest_paths );
CREATE_LOCAL_PROFILER( processing, shortest_paths );

ShortestPaths<ShortcutGraph, DijkstraResult> shortestPaths(
	const ShortcutGraph&  graph,
	ShortcutGraph::Vertex source,
	double                maxDist,
	double                minDist,
	double                maxEdge
)
{
	START_PROFILER( total, shortest_paths );

	START_PROFILER( dijkstra, shortest_paths );

	auto paths = dijkstra<ShortcutGraph, DijkstraResult>( graph, source, [&graph, maxDist]( const auto v, const auto e, const double dist ) {
		if (dist > maxDist) { return PredicateResponse::FALSE; }
		return PredicateResponse::TRUE;
	} );

	STOP_PROFILER( dijkstra, shortest_paths );


	START_PROFILER( processing, shortest_paths );

	paths.vertexMap( [&graph, minDist, maxEdge, &paths]( const auto& result ) {
		if (result.distance() < minDist)
		{
			paths.filter( result.vertex() );
			return false;
		}

		Vec<ShortcutGraph::Vertex> path;
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

	STOP_PROFILER( processing, shortest_paths );

	STOP_PROFILER( total, shortest_paths );
	return paths;
}

bool witnessSearch(
	const ShortcutGraph&  graph,
	ShortcutGraph::Vertex source,
	ShortcutGraph::Vertex target,
	ShortcutGraph::Vertex avoid,
	double                maxDist
)
{
	bool witnessFound = false;
	dijkstra<ShortcutGraph, DijkstraResult>( graph, source, [&graph, target, avoid, maxDist, &witnessFound]( const auto v, const auto e, const double dist ) {
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
	const WeightedGraph& graph,
	WeightedGraph::Edge  e
)
{
	const auto source = graph.source( e );
	const auto target = graph.target( e );
	const double maxDist = graph[e].weight();

	bool useful = true;
	dijkstra<ShortcutGraph, DijkstraResult>( graph, source, [&graph, target, maxDist, &useful]( const auto v, const auto e, const double dist ) {
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

ShortestPaths<WeightedGraph, DijkstraResult> dijkstraSearch( const WeightedGraph& graph, WeightedGraph::Vertex source, WeightedGraph::Vertex target )
{
	DijkstraData<WeightedGraph, DijkstraResult> data( source );

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