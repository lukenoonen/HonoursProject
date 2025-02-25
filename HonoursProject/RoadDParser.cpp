#include "RoadDParser.h"
#include "Dijkstra.h"

#include <string>
#include <unordered_map>

RoadDParser::RoadDParser( FilePath filepath )
	: GraphParser( std::move( filepath ) )
{

}

Ptr<WeightedGraph> RoadDParser::createInternal( std::ifstream file ) const
{
	Map<Vertex, Vertex> map;
	Ptr<WeightedGraph> result = std::make_unique<WeightedGraph>();

	Str line;
	while (std::getline( file, line ))
	{
		if (line[0] == '#') { continue; }
		std::stringstream stream( line );
		Vertex source, target;
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
		while (result->numVertices() <= source)
		{
			result->addVertex( {} );
		}
		while (result->numVertices() <= target)
		{
			result->addVertex( {} );
		}

		if (weight == 0.0)
		{
			if (!map.contains( source ))
			{
				map[target] = source;
			}
		}
		else if (!result->edge( source, target ))
		{
			result->addEdge( source, target, { weight } );
		}
	}

	result->removeIsolatedVertices();
	result->normalize();

	EdgeSet<WeightedGraph::Edge> uselessEdges;
	result->edgeMap( [&result, &uselessEdges]( const auto e ) {
		if (!usefulEdge( *result, e )) { uselessEdges.insert( e ); }
		return false;
	} );

	result->removeEdges( uselessEdges );

	return result;
}

std::ios_base::openmode RoadDParser::getOpenMode() const
{
	return std::ios::in;
}

FACTORY_BEGIN_JSON( "road-d", RoadDParser, GraphParser )

	JSON_ARG( Str, filepath )

	FACTORY_FABRICATE( std::move( filepath ) )

FACTORY_END()