#include "ShortcutGraph.h"
#include "FilteredGraph.h"
#include "Filter.h"
#include "Dijkstra.h"

ShortcutGraph::ShortcutGraph( const WeightedGraph& source )
{

}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& source )
	: Graph<ShortcutVertex, ShortcutEdge>( source )
{
	edgeMap( [&source, this]( const edge_descriptor e ) {
		ShortcutEdge& edge = (*this)[e];
		const ShortcutEdge otherEdge = source[e];
		edge = ShortcutEdge{ &otherEdge };
		return false;
		} );
}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<vertex_descriptor>& discard )
{
	if (discard.size() == gPrev.numVertices()) { return; }

	std::unordered_set<vertex_descriptor> discardSet;

	ShortcutGraph temp( gPrev );
	FilteredGraph filtered( temp, Filter::Exclude{ discardSet }, Filter::KeepAll{} );

	for (const vertex_descriptor v : discard)
	{
		filtered.edgeMap( v, [v, &filtered, &temp]( const edge_descriptor e1 ) {
			filtered.edgeMap( v, [v, e1, &filtered, &temp]( const edge_descriptor e2 ) {
				if (e1 == e2) { return false; }
				const double throughWeight = filtered[e1].weight() + filtered[e2].weight();
				const vertex_descriptor v1 = filtered.other( e1, v );
				const vertex_descriptor v2 = filtered.other( e2, v );

				const bool hasWitnessPath = dijkstraWitnessSearch( filtered, v1, v2, v, throughWeight );
				if (hasWitnessPath)
				{
					temp.addEdge( v1, v2, ShortcutEdge{ filtered[e1], filtered[e2] } );
				}
				return false;
				} );
			return false;
			} );
		discardSet.insert( v );
	}

	copy( filtered );
}

ShortcutGraph::vertex_descriptor ShortcutGraph::fromSource( WeightedGraph::vertex_descriptor v ) const
{
	return (*this)[v].mapped();
}

ShortcutGraph::vertex_descriptor ShortcutGraph::toSource( WeightedGraph::vertex_descriptor v ) const
{
	return (*this)[v].mapped();
}

ShortcutVertex::ShortcutVertex( ShortcutGraph::vertex_descriptor mapped )
	: _mapped( mapped )
{

}

ShortcutGraph::vertex_descriptor ShortcutVertex::mapped() const
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