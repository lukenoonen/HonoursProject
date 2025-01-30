#include "ShortcutHierarchy.h"
#include "Dijkstra.h"

#include "DijkstraSolver.h"

namespace
{
	template <class Graph>
	class HierarchyResult : public DijkstraResult<Graph>
	{
	public:
		using Vertex = typename Graph::Vertex;
		using Edge = typename Graph::Edge;

	public:
		HierarchyResult() = default;

		HierarchyResult( Vertex source, size_t level );
		HierarchyResult(
			Vertex vertex,
			Vertex prev,
			Edge   edge,
			double distance,
			size_t level
		);

		size_t level() const;

	private:
		size_t _level;
	};

	template<class Graph>
	HierarchyResult<Graph>::HierarchyResult( Vertex source, size_t level )
		: DijkstraResult<Graph>( source ),
		  _level( level )
	{

	}

	template<class Graph>
	HierarchyResult<Graph>::HierarchyResult(
		Vertex vertex,
		Vertex prev,
		Edge   edge,
		double distance,
		size_t level
	)
		: DijkstraResult<Graph>( vertex, prev, edge, distance ),
		  _level( level )
	{

	}

	template<class Graph>
	size_t HierarchyResult<Graph>::level() const
	{
		return _level;
	}

	bool oalgo(
		const Vec<ShortcutGraph>& hierarchy,
		const Vec<size_t>& ladder,
		DijkstraData<ShortcutGraph, HierarchyResult>& first,
		DijkstraData<ShortcutGraph, HierarchyResult>& second,
		double& bestDistance
	)
	{
		const auto& ex = first.extract();
		const auto& graph = hierarchy[ex.level()];
		const auto u = graph.fromSource( ex.vertex() );

		graph.edgeMap( u, [&ex, &graph, u, &first, &second, &bestDistance, &ladder]( const auto e ) {
			const auto v = graph.other( e, u );
			const auto vS = graph.toSource( v );
			const double vDist = first.distance( vS );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			first.decrease( vS, ex.vertex(), e, newDist, ladder[vS] );
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

ShortcutHierarchy::ShortcutHierarchy( const WeightedGraph& g )
	: _ladder( g.numVertices(), 0 )
{
	_hierarchy.emplace_back( g );
}

void ShortcutHierarchy::extend( const Vec<ShortcutGraph::Vertex>& discard )
{
	const ShortcutGraph& current = top();

	for (const auto v : discard)
	{
		_ladder[current[v].mapped()] = _hierarchy.size() - 1;
	}

	_hierarchy.emplace_back( _hierarchy.back(), discard );
}

const ShortcutGraph& ShortcutHierarchy::top() const
{
	return _hierarchy.back();
}

double ShortcutHierarchy::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	DijkstraData<ShortcutGraph, HierarchyResult> forward( s, 0 );
	DijkstraData<ShortcutGraph, HierarchyResult> backward( t, 0 );

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch = true;
	bool backwardSearch = true;
	
	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = oalgo( _hierarchy, _ladder, forward, backward, bestDistance );
		}
		if (backwardSearch)
		{
			backwardSearch = oalgo( _hierarchy, _ladder, backward, forward, bestDistance );
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