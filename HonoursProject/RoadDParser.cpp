#include "RoadDParser.h"
#include "Dijkstra.h"

#include <string>
#include <unordered_map>

RoadDParser::RoadDParser( std::filesystem::path filepath )
	: GraphParser( std::move( filepath ) )
{

}

std::optional<WeightedGraph> RoadDParser::createInternal( std::ifstream file ) const
{
	std::unordered_map<VertexDescriptor, VertexDescriptor> map;
	WeightedGraph result;

	std::string line;
	while (std::getline( file, line ))
	{
		if (line[0] == '#') { continue; }
		std::stringstream stream( line );
		VertexDescriptor source, target;
		double weight;
		stream >> source >> target >> weight;

		if (auto search = map.find( source ); search != map.end())
		{
			source = search->second;
		}
		if (auto search = map.find( target ); search != map.end())
		{
			target = search->second;
		}

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

		if (weight == 0.0)
		{
			if (!map.contains( source ))
			{
				map[target] = source;
			}
		}
		else if (!result.edge( source, target ))
		{
			result.addEdge( source, target, { weight } );
		}
	}

	result.removeIsolatedVertices();
	result.normalize();

	WeightedGraph::EdgeVector uselessEdges;
	result.edgeMap( [&result, &uselessEdges]( const auto e ) {
		if (!usefulEdge( result, e )) { uselessEdges.push_back( e ); }
		return false;
	} );

	result.removeEdges( uselessEdges );

	return std::move( result );
}

std::ios_base::openmode RoadDParser::getOpenMode() const
{
	return std::ios::in;
}