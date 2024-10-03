#include "ShortcutGraph.h"

ShortcutGraph::ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<vertex_descriptor>& c )
{
	for (const vertex_descriptor v : c)
	{
		addVertex( ShortcutVertex{ v } );
	}

	std::vector<vertex_descriptor> difference;
	difference.reserve( gPrev.numVertices() - c.size() );
	// TODO: calculate difference. Is there a better way than using sets?

	for (const vertex_descriptor v : difference)
	{
		gPrev.edgeMap( v, [this, &gPrev, v]( const auto e1 ) {
			gPrev.edgeMap( v, [this, &gPrev, v, e1]( const auto e2 ) {
				if (e1 == e2) { return false; }
				const double throughWeight = gPrev[e1].weight() + gPrev[e2].weight();
				// TODO: is a shortest-paths search gaurenteed to have constant time? Are there any good hueristics to use?
				// Geisberger et al. assume that local searches will terminate quickly as edges are, by construction, shortest paths,
				// so it is likely that the fewest edges will produce the shortest paths, and there are two edges separating
				// v1 and v2. They propose some hueristics which can speed up searches. For instance, since all edges are shortest
				// paths, if v1 and v2 are connected by an edge, the shortest path does not go through v
				const bool through = false;
				if (through)
				{

				}
				} );
			} );
	}
}

ShortcutVertex::ShortcutVertex( ShortcutGraph::vertex_descriptor mapped )
	: _mapped( mapped )
{

}

ShortcutGraph::vertex_descriptor ShortcutVertex::mapped() const
{
	return _mapped;
}

template <class T>
std::vector<T> concatenate( const std::vector<T>& first, const std::vector<T>& second )
{
	std::vector<T> result;
	result.reserve( first.size() + second.size() );
	std::copy( first.begin(), first.end(), std::back_inserter( result ) );
	std::copy( second.begin(), second.end(), std::back_inserter( result ) );
	return result;
}

ShortcutEdge::ShortcutEdge( std::vector<ShortcutEdge*> path )
	: _path( std::move( path ) )
{
	_weight = 0.0;
	_maxEdge = 0.0;

	for (const ShortcutEdge* edge : _path)
	{
		_weight += edge->weight();
		if (_maxEdge < edge->maxEdge())
		{
			_maxEdge = edge->maxEdge();
		}
	}
}

ShortcutEdge::ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second )
	: ShortcutEdge( concatenate( first.path(), second.path() ) )
{

}

const std::vector<ShortcutEdge*>& ShortcutEdge::path() const
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