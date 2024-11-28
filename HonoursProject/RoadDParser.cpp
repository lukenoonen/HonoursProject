#include "RoadDParser.h"

#include <string>
#include <unordered_map>

RoadDParser::RoadDParser( std::filesystem::path filepath )
	: GraphParser( std::move( filepath ) )
{

}

std::optional<WeightedGraph> RoadDParser::createInternal( std::ifstream file ) const
{
	WeightedGraph result;

	std::string line;
	while (std::getline( file, line ))
	{
		if (line[0] == '#') { continue; }
		std::stringstream stream( line );
		WeightedGraph::VertexDescriptor source, target;
		double weight;
		stream >> source >> target >> weight;
		if (stream.fail())
		{
			return {};
		}
		while (result.numVertices() <= source)
		{
			result.addVertex( {} );
		}
		while (result.numVertices() <= target)
		{
			result.addVertex( {} );
		}
		result.addEdge( source, target, { weight } );
	}

	return result;
}

std::ios_base::openmode RoadDParser::getOpenMode() const
{
	return std::ios::in;
}
