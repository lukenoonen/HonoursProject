#include "ShortcutGraph.h"
#include "FilteredGraph.h"
#include "Copier.h"
#include "Filter.h"
#include "Dijkstra.h"

ShortcutGraph::ShortcutGraph( const WeightedGraph& source )
{

}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<vertex_descriptor>& discard )
{
	if (discard.size() == gPrev.numVertices()) { return; }

	std::unordered_set<vertex_descriptor> discardSet;

	ShortcutGraph temp( gPrev, Copier::All{}, Copier::Pointer{} );
	FilteredGraph filtered( temp, Filter::Exclude{ discardSet }, Filter::KeepAll{} );

	for (const vertex_descriptor v : discard)
	{
		filtered.edgeMap( v, [v, &filtered]( const edge_descriptor e1 ) {
			filtered.edgeMap( v, [v, e1, &filtered]( const edge_descriptor e2 ) {
				if (e1 == e2) { return false; }
				const double throughWeight = filtered[e1].weight() + filtered[e2].weight();
				const vertex_descriptor v1 = filtered.other( e1, v );
				const vertex_descriptor v2 = filtered.other( e2, v );

				// TODO: is a shortest-paths search gaurenteed to have constant time? Are there any good hueristics to use?
				// Geisberger et al. assume that local searches will terminate quickly as edges are, by construction, shortest paths,
				// so it is likely that the fewest edges will produce the shortest paths, and there are two edges separating
				// v1 and v2. They propose some hueristics which can speed up searches. For instance, since all edges are shortest
				// paths, if v1 and v2 are connected by an edge, the shortest path does not go through v

				const bool hasWitnessPath = dijkstraWitnessSearch( filtered, v1, v2, v, throughWeight );
				if (hasWitnessPath)
				{
					filtered.addEdge( v1, v2, ShortcutEdge{ filtered[v1], filtered[v2] } );
				}
				return false;
				} );
			return false;
			} );
		discardSet.insert( v );
	}

	copy( filtered, Copier::All{}, Copier::All{} );
}

template <class VertexCopier, class EdgeCopier>
ShortcutGraph::ShortcutGraph( const ShortcutGraph& other, VertexCopier vc, EdgeCopier ec )
	: Graph( other, vc, ec )
{

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