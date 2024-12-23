#include "CIHierarchy.h"
#include "Dijkstra.h"

#include <unordered_set>
#include <algorithm>

namespace
{
	using WeightedVertexSet    = WeightedGraph::VertexSet;
	using WeightedVertexVector = WeightedGraph::VertexVector;

	using WeightedEdgeSet    = WeightedGraph::EdgeSet;
	using WeightedEdgeVector = WeightedGraph::EdgeVector;

	using ShortcutVertexSet    = ShortcutGraph::VertexSet;
	using ShortcutVertexVector = ShortcutGraph::VertexVector;

	using ShortcutEdgeSet    = ShortcutGraph::EdgeSet;
	using ShortcutEdgeVector = ShortcutGraph::EdgeVector;


	WeightedEdgeVector sortEdgesByWeight( const WeightedGraph& g )
	{
		WeightedEdgeVector edges;
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

	ShortcutVertexSet gatherEdgeVertices( std::size_t fromIndex, const WeightedEdgeVector& edges, const WeightedGraph& g, const ShortcutGraph& current, double test )
	{
		ShortcutVertexSet vertices;
		for (std::size_t i = fromIndex; i < edges.size(); i++)
		{
			vertices.insert( current.fromSource( g.source( edges[i] ) ) );
			vertices.insert( current.fromSource( g.target( edges[i] ) ) );
		}
		return vertices;
	}

	ShortcutVertexSet selectVertices( double maxWeight, double prevMaxWeight, const ShortcutGraph& current )
	{
		ShortcutVertexSet vertices;

		const double minWeight = 0.75 * maxWeight;
		current.vertexMap( [&current, minWeight, maxWeight, prevMaxWeight, &vertices]( const auto origin ) {
			if (vertices.contains( origin )) { return false; }
			ShortestPaths<ShortcutGraph> paths = shortestPaths( current, origin, maxWeight, minWeight, prevMaxWeight );
			paths.vertexMap( [minWeight, &paths, &vertices]( const auto& result ) {
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

		return vertices;
	}

	/*std::optional<size_t> vertexCover(const ShortcutVertexSet& cover, double r, const ShortcutGraph& current)
	{
		const bool first = current.vertexMap( [&cover, &current, r]( const auto origin ) {
			ShortestPaths<ShortcutGraph> paths = shortestPaths( current, origin, r, r );
			return paths.vertexMap( [&cover, &paths]( const auto vertex, double distance ) {
				return paths.pathMap( vertex, [&cover]( auto vertex_, double distance_ ) {
					return cover.contains( vertex_ );
				} );
			} );
		} );
		if (!first) { return {}; }

		size_t second;

	}*/

	ShortcutVertexVector calculateDiscard( const ShortcutVertexSet& keepSet, const ShortcutGraph& current )
	{
		const std::size_t discardSize = current.numVertices() - keepSet.size();
		ShortcutVertexVector discard;
		discard.reserve( discardSize );

		current.vertexMap( [&keepSet, discardSize, &discard]( const auto v ) {
			if (!keepSet.contains( v )) { discard.push_back( v ); }
			return discard.size() == discardSize;
		} );

		return discard;
	}
}

CIHierarchy::CIHierarchy( const WeightedGraph& g )
	: _ladder( g.numVertices() )
{
	WeightedEdgeVector edges = sortEdgesByWeight( g );

	_hierarchy.emplace_back( g );

	double maxWeight = 8.0;
	double prevMaxWeight = 1.0;
	size_t minEdgeIndex = 0;
	while (true)
	{
		while (minEdgeIndex < edges.size() && g[edges[minEdgeIndex]].weight() <= prevMaxWeight)
		{
			minEdgeIndex++;
		}

		const ShortcutGraph& back = _hierarchy.back();

		ShortcutVertexSet keepSet = gatherEdgeVertices( minEdgeIndex, edges, g, back, maxWeight );
		ShortcutVertexSet selectedSet = selectVertices( maxWeight, prevMaxWeight, back );
		for (const auto v : selectedSet)
		{
			keepSet.insert( v );
		}

		if (keepSet.size() == 0) { break; }

		prevMaxWeight = maxWeight;
		maxWeight *= 8.0;

		if (keepSet.size() == back.numVertices()) { continue; }

		ShortcutVertexVector discard = calculateDiscard( keepSet, back );

		for (const auto v : discard)
		{
			_ladder[back[v].mapped()] = _hierarchy.size() - 1;
		}

		
		std::cout << "Adding shortcut graph\n";
		const double backLength = back.length();
		_hierarchy.emplace_back( _hierarchy.back(), discard );
		const ShortcutGraph& current = _hierarchy.back();
		const double currentLength = current.length();
		std::cout << currentLength << " total " << (currentLength < backLength ? "(less)" : "(more)") << "\n";
		std::cout << current.numVertices() << " vertices, " << current.numEdges() << " edges\n";
		std::cout << current.meanDegree() << " mean degree\n\n";
	}
}

void serialize( std::ostream& os, const CIHierarchy& data )
{
	serialize( os, data._hierarchy );
	serialize( os, data._ladder );
}

void deserialize( std::istream& is, CIHierarchy& data )
{
	deserialize( is, data._hierarchy );
	deserialize( is, data._ladder );
}