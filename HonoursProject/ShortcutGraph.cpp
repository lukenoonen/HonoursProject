#include "ShortcutGraph.h"
#include "Dijkstra.h"

ShortcutGraph::ShortcutGraph( const WeightedGraph& source )
{
	source.vertexMap( [&source, this]( const auto v ) {
		addVertex( { v } );
		return false;
	} );

	source.edgeMap( [&source, this]( const auto e ) {
		addEdge( source.source( e ), source.target( e ), { e, source[e].weight() } );
		return false;
	} );

	calculateMap();
}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& prev, const Vec<Vertex>& discard )
	: BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>( prev )
{
	Set<WeightedGraph::Vertex> mappedDiscard;
	for (const Vertex v : discard)
	{
		mappedDiscard.insert( get( v ).mapped() );
	}

	for (const Vertex v : discard)
	{
		Set<Vertex> processed;
		Vec<std::tuple<Vertex, Vertex, ShortcutEdge>> shortcuts;
		edgeMap( v, [v, this, &processed, &shortcuts]( const auto e1 ) {
			const Vertex v1 = other( e1, v );
			processed.insert( v1 );
			edgeMap( v, [v, e1, v1, this, &processed, &shortcuts]( const auto e2 ) {
				const Vertex v2 = other( e2, v );
				if (processed.contains( v2 )) { return false; }

				const double throughWeight = get( e1 ).weight() + get( e2 ).weight();
				const bool hasWitnessPath = witnessSearch( *this, v1, v2, v, throughWeight );
				if (!hasWitnessPath)
				{
					shortcuts.emplace_back( v1, v2, ShortcutEdge{ get( e1 ), get( e2 ) } );
				}
				return false;
			} );
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
}

ShortcutGraph::Vertex ShortcutGraph::fromSource( WeightedGraph::Vertex v ) const
{
	return _map.find( v )->second;
}

WeightedGraph::Vertex ShortcutGraph::toSource( ShortcutGraph::Vertex v ) const
{
	return get( v ).mapped();
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
	  _weight( weight ),
	  _maxEdge( weight )
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
	_maxEdge = std::max( first.maxEdge(), second.maxEdge() );
}

const ShortcutEdge::PathType& ShortcutEdge::path() const
{
	return _path;
}

double ShortcutEdge::weight() const
{
	return _weight;
}

double ShortcutEdge::maxEdge() const
{
	return _maxEdge;
}

void serialize( std::ostream& os, const ShortcutEdge& data )
{
	serialize( os, data._path );
	serialize( os, data._weight );
	serialize( os, data._maxEdge );
}

void deserialize( std::istream& is, ShortcutEdge& data )
{
	deserialize( is, data._path );
	deserialize( is, data._weight );
	deserialize( is, data._maxEdge );
}