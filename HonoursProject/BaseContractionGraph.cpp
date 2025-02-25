#include "BaseContractionGraph.h"

ShortcutEdge::ShortcutEdge( WeightedGraph::Edge e, double weight )
	: _path( { e } ),
	_weight( weight )
{

}

ShortcutEdge::ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second )
{
	const PathType& firstPath = first.path();
	const PathType& secondPath = second.path();

	_path.reserve( firstPath.size() + secondPath.size() );

	std::copy( firstPath.begin(), firstPath.end(), std::back_inserter( _path ) );
	std::copy( secondPath.begin(), secondPath.end(), std::back_inserter( _path ) );

	_weight = first.weight() + second.weight();
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