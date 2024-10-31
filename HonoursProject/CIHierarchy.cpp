#include "CIHierarchy.h"
#include "Dijkstra.h"

#include <unordered_set>

using EdgeVector   = std::vector<WeightedGraph::edge_descriptor>;

using VertexSet    = std::unordered_set<ShortcutGraph::vertex_descriptor>;
using VertexVector = std::vector<ShortcutGraph::vertex_descriptor>;

EdgeVector sortEdgesByWeight( const WeightedGraph& g )
{
	EdgeVector edges;
	edges.reserve( g.numEdges() );
	g.edgeMap( [&edges]( const WeightedGraph::edge_descriptor e ) {
		edges.push_back( e );
		return false;
		} );
	std::sort( edges.begin(), edges.end(), [&g]( const WeightedGraph::edge_descriptor e1, const WeightedGraph::edge_descriptor e2 ) {
		return g[e1].weight() < g[e2].weight();
		} );
	return edges;
}

VertexSet gatherEdgeVertices( std::size_t fromIndex, const EdgeVector& edges, const WeightedGraph& g, const ShortcutGraph& current )
{
	VertexSet vertices;
	for (std::size_t i = fromIndex; i < edges.size(); i++)
	{
		vertices.insert( current.fromSource( g.source( edges[i] ) ) );
		vertices.insert( current.fromSource( g.target( edges[i] ) ) );
	}
	return vertices;
}

VertexSet selectVertices( double maxWeight, double prevMaxWeight, const ShortcutGraph& current )
{
	VertexSet vertices;

	const double minWeight = 0.75 * maxWeight;
	current.vertexMap( [&current, minWeight, maxWeight, prevMaxWeight, &vertices]( ShortcutGraph::vertex_descriptor origin ) {
		ShortestPaths<ShortcutGraph> paths = dijkstraShortestPaths<ShortcutGraph>( current, origin, maxWeight, prevMaxWeight );
		paths.vertexMap( [minWeight, &paths, &vertices]( ShortcutGraph::vertex_descriptor vertex, double distance ) {
			if (distance < minWeight) { return false; }

			const double midpointTarget = distance * 0.5;
			double bestMidpointDistance = abs( midpointTarget - distance );
			ShortcutGraph::vertex_descriptor bestMidpoint = vertex;
			bool addMidpoint = true;
			paths.pathMap( vertex, [&vertices, midpointTarget, &bestMidpointDistance, &bestMidpoint, &addMidpoint]( ShortcutGraph::vertex_descriptor vertex_, double distance_ ) {
				if (vertices.contains( vertex_ ))
				{
					addMidpoint = false;
					return true;
				}

				const double midpointDistance = abs( midpointTarget - distance_ );
				if (midpointDistance < bestMidpointDistance)
				{
					bestMidpointDistance = midpointDistance;
					bestMidpoint = vertex_;
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

	return vertices;
}

VertexVector calculateDiscard( const VertexSet& keepSet, const ShortcutGraph& current )
{
	const std::size_t discardSize = current.numVertices() - keepSet.size();
	std::vector<ShortcutGraph::vertex_descriptor> discard;
	discard.reserve( discardSize );

	current.vertexMap( [&keepSet, discardSize, &discard]( const ShortcutGraph::vertex_descriptor v ) {
		if (keepSet.contains( v )) { discard.push_back( v ); }
		return discard.size() == discardSize;
		} );

	return discard;
}

CIHierarchy::CIHierarchy( const WeightedGraph& g )
	: _g( g ), _ladder( g.numVertices() )
{
	// First, get the edges sorted from lowest to heighest weight

	std::vector<WeightedGraph::edge_descriptor> edges = sortEdgesByWeight( g );

	double maxWeight = 1.0;
	double prevMaxWeight = 0.0;
	std::size_t minEdgeIndex = 0;
	while (false) // TODO: condition?
	{
		while (g[edges[minEdgeIndex]].weight() <= prevMaxWeight) { minEdgeIndex++; }

		const ShortcutGraph& back = _hierarchy.back();
		VertexSet keepSet = gatherEdgeVertices( minEdgeIndex, edges, g, back );
		VertexSet selectedSet = selectVertices( maxWeight, prevMaxWeight, back );

		VertexVector discard = calculateDiscard( keepSet, back );

		for (const ShortcutGraph::vertex_descriptor v : discard)
		{
			_ladder[back[v].mapped()] = _hierarchy.size() - 1;
		}
		_hierarchy.emplace_back( _hierarchy.back(), discard );

		prevMaxWeight = maxWeight;
		maxWeight *= 8.0;
	}
}