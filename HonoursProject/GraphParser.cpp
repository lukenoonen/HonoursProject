#include "GraphParser.h"

GraphParser::GraphParser( FilePath filepath )
	: _filepath( std::move( filepath ) )
{

}

Ptr<WeightedGraph> GraphParser::create() const
{
	std::ifstream file( _filepath, getOpenMode() );
	return createInternal( std::move( file ) );
}

JSON_DEFINE_FACTORY( GraphParser )