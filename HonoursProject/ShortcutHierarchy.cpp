#include "ShortcutHierarchy.h"
#include "Dijkstra.h"

namespace
{
	bool dijkstraStep(
		const Vec<ShortcutGraph>&                    hierarchy,
		const Vec<size_t>&                           ladder,
		DijkstraData<ShortcutGraph, DijkstraResult>& first,
		DijkstraData<ShortcutGraph, DijkstraResult>& second,
		double&                                      bestDistance,
		const Vec<double>&                           maxDists
	)
	{
		const auto& ex    = first.extract();
		const auto  uS    = ex.vertex();
		const auto& graph = hierarchy[ladder[uS]];
		const auto  u     = graph.fromSource( uS );

		graph.edgeMap( u, [&ex, &graph, u, uS, &first, &second, &bestDistance, &ladder, &maxDists]( const auto e ) {
			const auto v = graph.other( e, u );
			const auto vS = graph.toSource( v );
			if (first.closed( vS )) { return false; }
			const double vDist = first.distance( vS );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			if (newDist > maxDists[ladder[vS]]) { return false; }
			first.decrease( vS, uS, e, newDist );
			const double distance = newDist + second.distance( vS );
			if (bestDistance > distance)
			{
				bestDistance = distance;
			}
			return false;
		} );
		
		return !first.empty() && ex.distance() < bestDistance;
	}
}

ShortcutHierarchy::ShortcutHierarchy(
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
	: _ladder( source.numVertices(), 0 )
{
	_hierarchy.emplace_back( source, edges, edgeRange );
}

void ShortcutHierarchy::extend(
	const Vec<ShortcutGraph::Vertex>& discard,
	const WeightedGraph&              source,
	const Vec<WeightedGraph::Edge>&   edges,
	const Pair<size_t, size_t>&       edgeRange
)
{
	const ShortcutGraph& current = top();
	for (const auto v : discard)
	{
		_ladder[current.toSource( v )] = _hierarchy.size() - 1;
	}

	_hierarchy.emplace_back( current, source, edges, edgeRange );
}

ShortcutGraph::Contraction ShortcutHierarchy::contract( ShortcutGraph::Vertex v )
{
	return _hierarchy.back().contract( v );
}

void ShortcutHierarchy::applyContraction( ShortcutGraph::Contraction contraction )
{
	_hierarchy.back().applyContraction( std::move( contraction ) );
}

void ShortcutHierarchy::finalizeLayer()
{
	_hierarchy.back().finalize();
}

void ShortcutHierarchy::finalize()
{
	const ShortcutGraph& current = top();
	current.vertexMap( [&current, this]( const auto v ) {
		_ladder[current.toSource( v )] = _hierarchy.size() - 1;
		return false;
	} );
}

ShortcutGraph& ShortcutHierarchy::top()
{
	return _hierarchy.back();
}

size_t ShortcutHierarchy::level( ShortcutGraph::Vertex v ) const
{
	return _ladder[v];
}

size_t ShortcutHierarchy::levels() const
{
	return _hierarchy.size();
}

namespace
{
	Vec<double> calculateMaxDists( double scale, size_t size )
	{
		Vec<double> result;
		result.reserve( size );
		result.emplace_back( scale );
		for (size_t i = 1; i < size; i++)
		{
			result.emplace_back( result.back() * scale );
		}
		return result;
	}
}

double ShortcutHierarchy::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	if (s == t) { return 0.0; }

	static const Vec<double> maxDists = calculateMaxDists( 8.0, _hierarchy.size() );

	DijkstraData<ShortcutGraph, DijkstraResult> forward( s );
	DijkstraData<ShortcutGraph, DijkstraResult> backward( t );

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch = true;
	bool backwardSearch = true;
	
	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = dijkstraStep( _hierarchy, _ladder, forward, backward, bestDistance, maxDists );
		}
		if (backwardSearch)
		{
			backwardSearch = dijkstraStep( _hierarchy, _ladder, backward, forward, bestDistance, maxDists );
		}
	}

	return bestDistance;
}

void serialize( std::ostream& os, const ShortcutHierarchy& data )
{
	serialize( os, data._hierarchy );
	serialize( os, data._ladder );
}

void deserialize( std::istream& is, ShortcutHierarchy& data )
{
	deserialize( is, data._hierarchy );
	deserialize( is, data._ladder );
}