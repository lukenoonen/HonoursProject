#include "CIHierarchyBuilder.h"
#include "Dijkstra.h"
#include "Profiler.h"
#include "Logger.h"

#include <algorithm>
#include <chrono>

namespace
{
	Vec<WeightedGraph::Edge> sortEdgesByWeight( const WeightedGraph& g )
	{
		Vec<WeightedGraph::Edge> edges;
		edges.reserve( g.numEdges() );
		g.edgeMap( [&edges]( const auto e ) {
			edges.push_back( e );
			return false;
			} );
		std::sort( edges.begin(), edges.end(), [&g]( const auto e1, const auto e2 ) {
			return g[e1].weight() < g[e2].weight();
			} );
		return edges;
	}

	Set<ShortcutGraph::Vertex> gatherEdgeVertices(
		size_t                          fromIndex,
		const Vec<WeightedGraph::Edge>& edges,
		const WeightedGraph& g,
		const ShortcutGraph& current,
		double                          test
	)
	{
		Set<ShortcutGraph::Vertex> vertices;
		for (size_t i = fromIndex; i < edges.size(); i++)
		{
			vertices.insert( current.fromSource( g.source( edges[i] ) ) );
			vertices.insert( current.fromSource( g.target( edges[i] ) ) );
		}
		return vertices;
	}

	CREATE_PROFILER_SET( selectVertices );
	CREATE_GLOBAL_PROFILER( total, selectVertices );
	CREATE_LOCAL_PROFILER( shortestPaths, selectVertices );

	Set<ShortcutGraph::Vertex> selectVertices(
		double               maxWeight,
		double               prevMaxWeight,
		const ShortcutGraph& current
	)
	{
		START_PROFILER( total, selectVertices );
		Set<ShortcutGraph::Vertex> vertices;

		const double minWeight = 0.75 * maxWeight;
		current.vertexMap( [&current, minWeight, maxWeight, prevMaxWeight, &vertices]( const auto origin ) {
			if (vertices.contains( origin )) { return false; }

			START_PROFILER( shortestPaths, selectVertices );
			ShortestPaths<ShortcutGraph> paths = shortestPaths( current, origin, maxWeight, minWeight, prevMaxWeight );
			STOP_PROFILER( shortestPaths, selectVertices );

			paths.vertexMap( [&paths, &vertices]( const auto& result ) {
				if (vertices.contains( result.vertex() )) { return false; }

				const double midpointTarget = result.distance() * 0.5;
				double bestMidpointDistance = std::abs( midpointTarget - result.distance() );
				auto bestMidpoint = result.vertex();

				const bool addMidpoint = !paths.pathMap( result.vertex(), [&vertices, midpointTarget, &bestMidpointDistance, &bestMidpoint]( const auto result_ ) {
					if (vertices.contains( result_.vertex() )) { return true; }

					const double midpointDistance = std::abs( midpointTarget - result_.distance() );
					if (midpointDistance < bestMidpointDistance)
					{
						bestMidpointDistance = midpointDistance;
						bestMidpoint = result_.vertex();
					}

					return false;
					} );

				if (addMidpoint)
				{
					vertices.insert( bestMidpoint );
				}

				return false;
				} );

			return false;
			} );

		STOP_PROFILER( total, selectVertices );

		LOG_PROFILERS( selectVertices );
		CLEAR_PROFILERS( selectVertices );

		LOG_PROFILERS( shortestPaths );
		CLEAR_PROFILERS( shortestPaths );

		g_logger.debug( "\n" );

		return vertices;
	}

	Vec<ShortcutGraph::Vertex> calculateDiscard(
		const Set<ShortcutGraph::Vertex>& keepSet,
		const ShortcutGraph& current )
	{
		const size_t discardSize = current.numVertices() - keepSet.size();
		Vec<ShortcutGraph::Vertex> discard;
		discard.reserve( discardSize );

		current.vertexMap( [&keepSet, discardSize, &discard]( const auto v ) {
			if (!keepSet.contains( v )) { discard.push_back( v ); }
			return discard.size() == discardSize;
			} );

		return discard;
	}
}

CIHierarchyBuilder::CIHierarchyBuilder( FilePath filepath, const WeightedGraph& graph )
	: ShortcutHierarchyBuilder( std::move( filepath ), graph )
{

}

Ptr<ShortcutHierarchy> CIHierarchyBuilder::buildInternal() const
{
	const WeightedGraph& g = graph();

	Ptr<ShortcutHierarchy> result = std::make_unique<ShortcutHierarchy>( g );
	Vec<WeightedGraph::Edge> edges = sortEdgesByWeight( g );

	double maxWeight = 8.0;
	double prevMaxWeight = 1.0;
	size_t minEdgeIndex = 0;
	while (true)
	{
		using std::chrono::high_resolution_clock;

		while (minEdgeIndex < edges.size() && g[edges[minEdgeIndex]].weight() <= prevMaxWeight)
		{
			minEdgeIndex++;
		}

		const ShortcutGraph& back = result->top();

		Set<ShortcutGraph::Vertex> keepSet
			= gatherEdgeVertices( minEdgeIndex, edges, g, back, maxWeight );

		Set<ShortcutGraph::Vertex> selectedSet
			= selectVertices( maxWeight, prevMaxWeight, back );

		for (const auto v : selectedSet)
		{
			keepSet.insert( v );
		}

		if (keepSet.size() == 0) { break; }

		prevMaxWeight = maxWeight;
		maxWeight *= 8.0;

		if (keepSet.size() == back.numVertices()) { continue; }


		Vec<ShortcutGraph::Vertex> discard = calculateDiscard( keepSet, back );

		result->extend( discard );
	}
	
	return result;
}

FACTORY_BEGIN_JSON_PARAM( "ci_hierarchy", CIHierarchyBuilder, PathSolverBuilder, (const WeightedGraph&), (const WeightedGraph& g) )
	
	JSON_ARG_FALLBACK( Str, filepath, "" )
	
	FACTORY_FABRICATE( std::move( filepath ), g )

FACTORY_END()