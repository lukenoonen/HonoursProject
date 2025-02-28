#include "RoadDParser.h"
#include "Dijkstra.h"

RoadDParser::RoadDParser( FilePath filepath, FilePath datapath )
	: GraphParser( std::move( filepath ) ),
	  _datapath( std::move( datapath ) )
{

}


Ptr<WeightedGraph> RoadDParser::createInternal() const
{
	std::ifstream file( _datapath, std::ios::in );

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
			return nullptr;
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

	return result;
}

FACTORY_BEGIN_JSON( "road-d", RoadDParser, GraphParser )

	JSON_ARG( Str, filepath )
	JSON_ARG( Str, datapath )

	FACTORY_FABRICATE( std::move( filepath ), std::move( datapath ) )

FACTORY_END()