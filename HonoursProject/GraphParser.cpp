#include "GraphParser.h"
#include "Dijkstra.h"

#include "Logger.h"

GraphParser::GraphParser( FilePath cachePath )
	: _cachePath( std::move( cachePath ) )
{

}

Ptr<WeightedGraph> GraphParser::create() const
{
	USING_GRAPH( WeightedGraph );

	g_logger.log( "Creating weighted graph...\n" );
	if (!_cachePath.empty())
	{
		Ptr<WeightedGraph> result = std::make_unique<WeightedGraph>();
		const bool success = deserialize( _cachePath, *result );
		if (success) { return result; }
		g_logger.log( "Failed to load cached weighted graph from {}\n", _cachePath.string() );
	}

	g_logger.log( "Building weighted graph...\n" );

	Ptr<WeightedGraph> result = createInternal();

	g_logger.log( "Identifying largest connected component...\n" );
	const auto& [inside, outside] = result->largestConnectedComponent();
	result->removeVertices( outside );

	g_logger.log( "Identifying all useless edges...\n" );
	EdgeSet<WeightedGraph::Edge> uselessEdges;
	result->edgeMap( [&result, &uselessEdges]( const auto e ) {
		if (!usefulEdge( *result, e )) { uselessEdges.insert( e ); }
		return false;
	} );

	g_logger.log( "Removing all useless edges...\n" );
	result->removeEdges( uselessEdges );

	g_logger.log( "Normalizing graph...\n" );
	result->normalize();

	if (!_cachePath.empty())
	{
		g_logger.log( "Caching graph...\n" );
		const bool success = serialize( _cachePath, *result );
		if (!success)
		{
			g_logger.log( "Failed to cache weighted graph at {}\n", _cachePath.string() );
		}
	}

	return result;
}

JSON_DEFINE_FACTORY( GraphParser )