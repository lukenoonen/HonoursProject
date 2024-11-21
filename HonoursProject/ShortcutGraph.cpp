#include "ShortcutGraph.h"
#include "Filter.h"
#include "Dijkstra.h"

ShortcutGraph::ShortcutGraph( const WeightedGraph& source )
{

}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& source )
	: BaseGraph<ShortcutVertex, ShortcutEdge>( source )
{
	edgeMap( [&source, this]( const EdgeDescriptor e ) {
		ShortcutEdge& edge = (*this)[e];
		const ShortcutEdge otherEdge = source[e];
		edge = ShortcutEdge{ &otherEdge };
		return false;
		} );
}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& prev, const std::vector<VertexDescriptor>& discard )
	: ShortcutGraph( prev )
{
	for (const VertexDescriptor v : discard)
	{
		edgeMap( v, [v, this]( const EdgeDescriptor e1 ) {
			edgeMap( v, [v, e1, this]( const EdgeDescriptor e2 ) {
				if (e1 == e2) { return false; }
				const double throughWeight = get( e1 ).weight() + get( e2 ).weight();
				const VertexDescriptor v1 = other( e1, v );
				const VertexDescriptor v2 = other( e2, v );

				const bool hasWitnessPath = dijkstraWitnessSearch( *this, v1, v2, v, throughWeight );
				if (hasWitnessPath)
				{
					addEdge( v1, v2, ShortcutEdge{ get( e1 ), get( e2 ) } );
				}
				return false;
				} );
			// TODO: delete e1
			return false;
			} );
	}

	removeVertices( discard );
}

ShortcutGraph::VertexDescriptor ShortcutGraph::fromSource( WeightedGraph::VertexDescriptor v ) const
{
	return (*this)[v].mapped();
}

ShortcutGraph::VertexDescriptor ShortcutGraph::toSource( WeightedGraph::VertexDescriptor v ) const
{
	return (*this)[v].mapped();
}

ShortcutVertex::ShortcutVertex( ShortcutGraph::VertexDescriptor mapped )
	: _mapped( mapped )
{

}

ShortcutGraph::VertexDescriptor ShortcutVertex::mapped() const
{
	return _mapped;
}

ShortcutEdge::ShortcutEdge( double weight )
	: _path( {} ), _weight( weight ), _maxEdge( weight )
{

}

ShortcutEdge::ShortcutEdge( const ShortcutEdge* prev )
	: _path( { prev } ), _weight( prev->weight() ), _maxEdge( prev->maxEdge() )
{

}

ShortcutEdge::ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second )
{
	const path_type& firstPath = first.path();
	const path_type& secondPath = second.path();

	_path.reserve( firstPath.size() + secondPath.size() );

	std::copy( firstPath.begin(), firstPath.end(), std::back_inserter( _path ) );
	std::copy( secondPath.begin(), secondPath.end(), std::back_inserter( _path ) );

	_weight = first.weight() + second.weight();
	_maxEdge = std::max( first.maxEdge(), second.maxEdge() );
	for (const ShortcutEdge* edge : _path)
	{
		_weight += edge->weight();
		if (_maxEdge < edge->maxEdge())
		{
			_maxEdge = edge->maxEdge();
		}
	}
}

const ShortcutEdge::path_type& ShortcutEdge::path() const
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