#include "GraphParser.h"

#include "Logger.h"

GraphParser::GraphParser( FilePath filepath )
	: _filepath( std::move( filepath ) )
{

}

Ptr<WeightedGraph> GraphParser::create() const
{
	g_logger.debug( "Creating graph from file {}...\n", _filepath.string() );
	std::ifstream file( _filepath, getOpenMode() );
	return createInternal( std::move( file ) );
}

JSON_DEFINE_FACTORY( GraphParser )