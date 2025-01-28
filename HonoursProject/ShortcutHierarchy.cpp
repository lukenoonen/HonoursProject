#include "ShortcutHierarchy.h"
#include "Dijkstra.h"

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

double ShortcutHierarchy::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	DijkstraData<WeightedGraph, HierarchyResult> forward( s, _ladder[s] );
	DijkstraData<WeightedGraph, HierarchyResult> backward( t, _ladder[t] );

	double dist = std::numeric_limits<double>::infinity();

	/*
	while (!forward.empty() && !backward.empty())
	{
		const auto& exF    = forward.extract();
		const auto& graphF = _hierarchy[exF.level()];

		const auto& exB    = backward.extract();
		const auto& graphB = _hierarchy[exB.level()];
		graph.edgeMap( ex.vertex(), [&ex, &graph, &data]( const auto e ) {
			const auto v = graph.other( e, ex.vertex() );
			const double vDist = data.distance( v );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			data.decrease( v, ex.vertex(), e, newDist );
			return false;
		} );
	}

	return data.results();*/
	return 0.0;
}

const ShortcutGraph& ShortcutHierarchy::top() const
{
	return _hierarchy.back();
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

// FACTORY_DEFINE_CACHE( "cached_shortcut_hierarchy", ShortcutHierarchy, PathSolver )