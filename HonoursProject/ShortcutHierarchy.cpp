#include "ShortcutHierarchy.h"

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
	return 0.0; // TODO: finish
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