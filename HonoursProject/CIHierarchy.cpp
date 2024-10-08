#include "CIHierarchy.h"

#include <unordered_set>

CIHierarchy::CIHierarchy( const WeightedGraph& g )
	: _g( g ), _ladder( g.numVertices() )
{
	// First, get the edges sorted from lowest to heighest weight
	std::vector<WeightedGraph::edge_descriptor> edges;
	edges.reserve( g.numEdges() );
	g.edgeMap( [&edges]( const WeightedGraph::edge_descriptor e ) {
		edges.push_back( e );
		return false;
		} );
	std::sort( edges.begin(), edges.end(), [&g]( const WeightedGraph::edge_descriptor e1, const WeightedGraph::edge_descriptor e2 ) {
		return g[e1].weight() < g[e2].weight();
		} );
	
	double maxWeight = 1.0;
	double prevMaxWeight = 0.0;
	std::size_t minEdgeIndex = 0;
	while (false) // TODO: condition?
	{
		while (g[edges[minEdgeIndex]].weight() <= prevMaxWeight) { minEdgeIndex++; }

		const ShortcutGraph& back = _hierarchy.back();
		std::unordered_set<ShortcutGraph::vertex_descriptor> keepSet;
		for (std::size_t i = minEdgeIndex; i < edges.size(); i++)
		{
			keepSet.insert( back.fromSource( g.source( edges[i] ) ) );
			keepSet.insert( back.fromSource( g.target( edges[i] ) ) );
		}

		// Populate keepset using IC method

		const std::size_t discardSize = back.numVertices() - keepSet.size();
		std::vector<ShortcutGraph::vertex_descriptor> discard;
		discard.reserve( discardSize );

		back.vertexMap( [&keepSet, discardSize, &discard]( const ShortcutGraph::vertex_descriptor v ) {
			if (keepSet.contains( v )) { discard.push_back( v ); }
			return discard.size() == discardSize;
			} );

		for (const ShortcutGraph::vertex_descriptor v : discard)
		{
			_ladder[back[v].mapped()] = _hierarchy.size() - 1;
		}
		_hierarchy.emplace_back( _hierarchy.back(), discard );

		prevMaxWeight = maxWeight;
		maxWeight *= 8.0;
	}
}