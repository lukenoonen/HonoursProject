#include "GraphParser.h"

GraphParser::GraphParser( std::filesystem::path filepath )
	: _filepath( std::move( filepath ) )
{

}

std::optional<WeightedGraph> GraphParser::create() const
{
	std::ifstream file( _filepath, getOpenMode() );
	if (!file) { return {}; }
	return createInternal( std::move( file ) );
}