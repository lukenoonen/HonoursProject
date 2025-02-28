#include "GraphParser.h"
#include "Dijkstra.h"

#include "Logger.h"

GraphParser::GraphParser( FilePath cachePath )
	: _cachePath( std::move( cachePath ) )
{

}

Ptr<WeightedGraph> GraphParser::create() const
{
	g_logger.debug( "Creating weighted graph...\n" );
	if (!_cachePath.empty())
	{
		Ptr<WeightedGraph> result = std::make_unique<WeightedGraph>();
		const bool success = deserialize( _cachePath, *result );
		if (success) { return result; }
		g_logger.debug( "No cache found at {}, rebuilding...\n", _cachePath.string() );
	}

	Ptr<WeightedGraph> result = createInternal();

	Vec<Set<Vertex>> ccs = result->connectedComponents();
	const Set<Vertex>& island
		= *std::max_element( ccs.begin(), ccs.end(), []( const auto a, const auto b ) {
		return a.size() < b.size();
	} );
	Set<Vertex> disconnected;
	result->vertexMap( [&island, &disconnected]( const auto v ) {
		if (!island.contains( v )) { disconnected.insert( v ); }
		return false;
	} );

	result->removeVertices( disconnected );

	EdgeSet<WeightedGraph::Edge> uselessEdges;
	result->edgeMap( [&result, &uselessEdges]( const auto e ) {
		if (!usefulEdge( *result, e )) { uselessEdges.insert( e ); }
		return false;
	} );
	result->removeEdges( uselessEdges );

	result->normalize();

	if (!_cachePath.empty())
	{
		const bool success = serialize( _cachePath, *result );
		if (!success)
		{
			g_logger.debug( "Failed to cache finalized graph at {}\n", _cachePath.string() );
		}
	}

	return result;
}

JSON_DEFINE_FACTORY( GraphParser )