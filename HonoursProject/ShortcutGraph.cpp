#include "ShortcutGraph.h"
#include "Dijkstra.h"

CREATE_GLOBAL_PROFILER( total, shortcut_graph );
CREATE_GLOBAL_PROFILER( witness_search, shortcut_graph );

ShortcutGraph::ShortcutGraph(
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
{
	START_PROFILER( total, shortcut_graph );

	source.vertexMap( [&source, this]( const auto v ) {
		addVertex( { v } );
		return false;
	} );

	calculateMap();

	addSourceEdges( source, edges, edgeRange );

	STOP_PROFILER( total, shortcut_graph );
}

ShortcutGraph::ShortcutGraph(
	const ShortcutGraph&            prev,
	const Vec<Vertex>&              discard,
	double                          maxEdge,
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
	: ShortcutGraph( prev )
{
	START_PROFILER( total, shortcut_graph );

	addSourceEdges( source, edges, edgeRange );

	for (const Vertex v : discard)
	{
		Vec<std::tuple<Vertex, Vertex, ShortcutEdge>> shortcuts;
		neighbourEdgeMap( v, [v, this, maxEdge, &shortcuts]( const auto e1, const auto e2 ) {
			const Vertex v1 = other( e1, v );
			const Vertex v2 = other( e2, v );
			const double throughWeight = get( e1 ).weight() + get( e2 ).weight();
			if (throughWeight > maxEdge) { return false; }
			START_PROFILER( witness_search, shortcut_graph );
			const bool hasWitnessPath = witnessSearch( *this, v1, v2, v, throughWeight );
			STOP_PROFILER( witness_search, shortcut_graph );
			if (!hasWitnessPath)
			{
				shortcuts.emplace_back( v1, v2, ShortcutEdge{ get( e1 ), get( e2 ) } );
			}
			return false;
		} );

		_filter.filter.insert( v );

		for (auto& [v1, v2, edge] : shortcuts)
		{
			addEdge( v1, v2, std::move( edge ) );
		}
	}

	_filter.filter.clear();

	removeVertices( discard );

	calculateMap();

	STOP_PROFILER( total, shortcut_graph );
}

ShortcutGraph::Vertex ShortcutGraph::fromSource( WeightedGraph::Vertex v ) const
{
	return _map.find( v )->second;
}

WeightedGraph::Vertex ShortcutGraph::toSource( ShortcutGraph::Vertex v ) const
{
	return get( v ).mapped();
}

void ShortcutGraph::addSourceEdges(
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
{
	for (size_t i = edgeRange.first; i < edgeRange.second; i++)
	{
		const auto& e = edges[i];
		const auto u = fromSource( source.source( e ) );
		const auto v = fromSource( source.target( e ) );
		addEdge( u, v, { e, source[e].weight() } );
	}
}

void ShortcutGraph::calculateMap()
{
	_map.clear();
	vertexMap( [this]( const auto v ) {
		_map[get( v ).mapped()] = v;
		return false;
	} );
}

void serialize( std::ostream& os, const ShortcutGraph& data )
{
	using BaseType = BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>;
	serialize( os, data._map );
	serialize( os, (const BaseType&)(data) );
}

void deserialize( std::istream& is, ShortcutGraph& data )
{
	using BaseType = BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>;
	deserialize( is, data._map );
	deserialize( is, (BaseType&)(data) );
}

ShortcutVertex::ShortcutVertex( WeightedGraph::Vertex mapped )
	: _mapped( mapped )
{

}

WeightedGraph::Vertex ShortcutVertex::mapped() const
{
	return _mapped;
}

void serialize( std::ostream& os, const ShortcutVertex& data )
{
	serialize( os, data._mapped );
}

void deserialize( std::istream& is, ShortcutVertex& data )
{
	deserialize( is, data._mapped );
}

ShortcutEdge::ShortcutEdge( WeightedGraph::Edge e, double weight )
	: _path( { e } ),
	  _weight( weight )
{

}

ShortcutEdge::ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second )
{
	const PathType& firstPath  = first.path();
	const PathType& secondPath = second.path();

	_path.reserve( firstPath.size() + secondPath.size() );

	std::copy( firstPath.begin(), firstPath.end(), std::back_inserter( _path ) );
	std::copy( secondPath.begin(), secondPath.end(), std::back_inserter( _path ) );

	_weight  = first.weight() + second.weight();
}

const ShortcutEdge::PathType& ShortcutEdge::path() const
{
	return _path;
}

double ShortcutEdge::weight() const
{
	return _weight;
}

void serialize( std::ostream& os, const ShortcutEdge& data )
{
	serialize( os, data._path );
	serialize( os, data._weight );
}

void deserialize( std::istream& is, ShortcutEdge& data )
{
	deserialize( is, data._path );
	deserialize( is, data._weight );
}