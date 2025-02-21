#include "ShortcutHierarchy.h"
#include "Dijkstra.h"

namespace
{
	bool dijkstraStep(
		const Vec<ShortcutGraph>&                     hierarchy,
		const Vec<size_t>&                            ladder,
		DijkstraData<ShortcutGraph, DijkstraResult>& first,
		DijkstraData<ShortcutGraph, DijkstraResult>& second,
		double&                                       bestDistance
	)
	{
		const auto& ex    = first.extract();
		const auto uS     = ex.vertex();
		const auto& graph = hierarchy[ladder[uS]];
		const auto u      = graph.fromSource( uS );

		graph.edgeMap( u, [&ex, &graph, u, uS, &first, &second, &bestDistance]( const auto e ) {
			const auto v = graph.other( e, u );
			const auto vS = graph.toSource( v );
			if (first.closed( vS )) { return false; }
			const double vDist = first.distance( vS );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
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
	double                            maxEdge,
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

	_hierarchy.emplace_back(
		_hierarchy.back(),
		discard,
		maxEdge,
		source,
		edges,
		edgeRange
	);
}

void ShortcutHierarchy::finalize()
{
	const ShortcutGraph& current = top();
	current.vertexMap( [&current, this]( const auto v ) {
		_ladder[current.toSource( v )] = _hierarchy.size() - 1;
		return false;
	} );
}

const ShortcutGraph& ShortcutHierarchy::top() const
{
	return _hierarchy.back();
}

double ShortcutHierarchy::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	if (s == t) { return 0.0; }

	DijkstraData<ShortcutGraph, DijkstraResult> forward( s );
	DijkstraData<ShortcutGraph, DijkstraResult> backward( t );

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch = true;
	bool backwardSearch = true;
	
	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = dijkstraStep( _hierarchy, _ladder, forward, backward, bestDistance );
		}
		if (backwardSearch)
		{
			backwardSearch = dijkstraStep( _hierarchy, _ladder, backward, forward, bestDistance );
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