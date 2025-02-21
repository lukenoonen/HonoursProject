#include "CIHierarchyBuilder.h"
#include "Dijkstra.h"
#include "Profiler.h"
#include "Logger.h"

#include <algorithm>

namespace
{
	Vec<WeightedGraph::Edge> sortEdgesByWeight( const WeightedGraph& source )
	{
		Vec<WeightedGraph::Edge> edges;
		edges.reserve( source.numEdges() );
		source.edgeMap( [&edges]( const auto e ) {
			edges.push_back( e );
			return false;
		} );
		std::sort( edges.begin(), edges.end(), [&source]( const auto e1, const auto e2 ) {
			return source[e1].weight() < source[e2].weight();
		} );
		return edges;
	}

	size_t incrementEdgeIndexToWeight(
		double                          weight,
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		size_t                          index
	)
	{
		while (index < edges.size() && source[edges[index]].weight() <= weight)
		{
			index++;
		}

		return index;
	}

	Set<ShortcutGraph::Vertex> gatherEdgeVertices(
		size_t                          fromIndex,
		const Vec<WeightedGraph::Edge>& edges,
		const WeightedGraph&            g,
		const ShortcutGraph&            current
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


	CREATE_PROFILER_SET( select_vertices );
	CREATE_GLOBAL_PROFILER( total, select_vertices );
	CREATE_GLOBAL_PROFILER( shortest_paths, select_vertices );
	CREATE_GLOBAL_PROFILER( processing, select_vertices );

	Set<ShortcutGraph::Vertex> selectVertices(
		double               maxWeight,
		const ShortcutGraph& current
	)
	{
		START_PROFILER( total, select_vertices );
		Set<ShortcutGraph::Vertex> vertices;

		const double minWeight = 0.75 * maxWeight;
		current.vertexMap( [&current, minWeight, maxWeight, &vertices]( const auto origin ) {
			if (vertices.contains( origin )) { return false; }

			START_PROFILER( shortest_paths, select_vertices );
			const auto paths = shortestPaths( current, origin, maxWeight, minWeight );
			STOP_PROFILER( shortest_paths, select_vertices );

			START_PROFILER( processing, select_vertices );

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

			STOP_PROFILER( processing, select_vertices );

			return false;
		} );

		STOP_PROFILER( total, select_vertices );

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
	: CachedPathSolverBuilder<ShortcutHierarchy>( std::move( filepath ), graph )
{

}

CREATE_GLOBAL_PROFILER( total, ci_builder );
CREATE_GLOBAL_PROFILER( select_vertices, ci_builder );
CREATE_GLOBAL_PROFILER( extend, ci_builder );

Ptr<ShortcutHierarchy> CIHierarchyBuilder::buildInternal() const
{
	g_logger.debug( "Constructing CI hierarchy...\n" );

	START_PROFILER( total, ci_builder );
	 
	const WeightedGraph&     source = graph();
	Vec<WeightedGraph::Edge> edges = sortEdgesByWeight( source );

	double maxEdge = 0.125;
	double maxWeight = 1.0;

	size_t maxEdgeIndex   = 0;
	size_t maxWeightIndex = incrementEdgeIndexToWeight( maxWeight, source, edges, maxEdgeIndex );

	Ptr<ShortcutHierarchy> result = std::make_unique<ShortcutHierarchy>( source, edges, Pair{ maxEdgeIndex, maxWeightIndex } );

	while (true)
	{
		maxEdge = maxWeight;
		maxWeight *= 8.0;

		maxEdgeIndex   = maxWeightIndex;
		maxWeightIndex = incrementEdgeIndexToWeight( maxWeight, source, edges, maxEdgeIndex );

		const ShortcutGraph& top = result->top();

		Set<ShortcutGraph::Vertex> keepSet
			= gatherEdgeVertices( maxEdgeIndex, edges, source, top );

		START_PROFILER( select_vertices, ci_builder );
		Set<ShortcutGraph::Vertex> selectedSet
			= selectVertices( maxWeight, top );
		STOP_PROFILER( select_vertices, ci_builder );

		for (const auto v : selectedSet)
		{
			keepSet.insert( v );
		}

		if (keepSet.size() == 0) { break; }

		Vec<ShortcutGraph::Vertex> discard = calculateDiscard( keepSet, top );

		START_PROFILER( extend, ci_builder );
		result->extend( discard, maxWeight, source, edges, Pair{ maxEdgeIndex, maxWeightIndex } );
		STOP_PROFILER( extend, ci_builder );
	}

	result->finalize();

	STOP_PROFILER( total, ci_builder );

	LOG_PROFILERS( ci_builder );
	CLEAR_PROFILERS( ci_builder );
	g_logger.debug( "\n" );

	LOG_PROFILERS( select_vertices );
	CLEAR_PROFILERS( select_vertices );
	g_logger.debug( "\n" );

	LOG_PROFILERS( shortest_paths );
	CLEAR_PROFILERS( shortest_paths );
	g_logger.debug( "\n" );

	LOG_PROFILERS( shortcut_graph );
	CLEAR_PROFILERS( shortcut_graph );
	g_logger.debug( "\n" );
	
	return result;
}

FACTORY_BEGIN_JSON_PARAM( "ci_hierarchy", CIHierarchyBuilder, PathSolverBuilder, (const WeightedGraph&), (const WeightedGraph& g) )
	
	JSON_ARG_FALLBACK( Str, filepath, "" )
	
	FACTORY_FABRICATE( std::move( filepath ), g )

FACTORY_END()