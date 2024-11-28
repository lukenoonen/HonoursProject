#include "ShortcutGraph.h"
#include "Dijkstra.h"

ShortcutGraph::ShortcutGraph( const WeightedGraph& source )
{
	size_t index = 0;
	while (numVertices() < source.numVertices())
	{
		addVertex( { index++ } );
	}

	source.edgeMap( [&source, this]( const WeightedGraph::EdgeDescriptor e ) {
		addEdge( source.source( e ), source.target( e ), { source[e].weight() } );
		return false;
	} );

	calculateMap();
}

ShortcutGraph::ShortcutGraph( const ShortcutGraph& prev, const std::vector<VertexDescriptor>& discard )
	: BaseGraph<ShortcutVertex, ShortcutEdge>( prev )
{
	edgeMap( [&prev, this]( const EdgeDescriptor e ) {
		ShortcutEdge& edge = (*this)[e];
		const ShortcutEdge otherEdge = prev[e];
		edge = ShortcutEdge{ &otherEdge };
		return false;
	} );

	std::unordered_set<VertexDescriptor> discarded;
	for (const VertexDescriptor v : discard)
	{
		edgeMap( v, [v, &discarded, this]( const EdgeDescriptor e1 ) {
			if (discarded.contains( target( e1 ) )) { return false; }

			edgeMap( v, [v, &discarded, e1, this]( const EdgeDescriptor e2 ) {
				if (discarded.contains( target( e2 ) )) { return false; }

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
			return false;
		} );
		discarded.insert( v );
	}

	removeVertices( discard );

	calculateMap();
}

ShortcutGraph::VertexDescriptor ShortcutGraph::fromSource( WeightedGraph::VertexDescriptor v ) const
{
	return _map.find( v )->second;
}

WeightedGraph::VertexDescriptor ShortcutGraph::toSource( ShortcutGraph::VertexDescriptor v ) const
{
	return get( v ).mapped();
}

void ShortcutGraph::calculateMap()
{
	_map.clear();
	vertexMap( [this]( const WeightedGraph::VertexDescriptor v ) {
		_map[get( v ).mapped()] = v;
		return false;
	} );
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
	const PathType& firstPath = first.path();
	const PathType& secondPath = second.path();

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