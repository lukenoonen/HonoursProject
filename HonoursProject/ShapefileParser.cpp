#include "ShapefileParser.h"
#include "Dijkstra.h"

#include <shapefil.h>

#include <regex>

ShapefileParser::ShapefileParser( FilePath filepath, FilePath directory )
	: GraphParser( std::move( filepath ) ),
	  _directory( std::move( directory ) )
{

}

Ptr<WeightedGraph> ShapefileParser::createInternal() const
{
	namespace fs = std::filesystem;

	if (!fs::is_directory( _directory )) { return nullptr; }

	Ptr<WeightedGraph> result = std::make_unique<WeightedGraph>();

	Map<Str, size_t> nodeMap;
	for (const auto& entry : fs::directory_iterator( _directory ))
	{
		FilePath path = entry.path();
		static const std::regex re( "[A-Z][A-Z]_RoadLink.dbf" );
		const bool match = std::regex_match( path.filename().string(), re );
		if (!match) { continue; }

		DBFHandle handle = DBFOpen( path.string().c_str(), "rb" );

		const int recordCount = DBFGetRecordCount( handle );

		const int lengthIndex = DBFGetFieldIndex( handle, "length" );
		const int startNodeIndex = DBFGetFieldIndex( handle, "startNode" );
		const int endNodeIndex = DBFGetFieldIndex( handle, "endNode" );

		for (int shape = 0; shape < recordCount; shape++)
		{
			const int length = DBFReadIntegerAttribute( handle, shape, lengthIndex );
			const Str startNode = DBFReadStringAttribute( handle, shape, startNodeIndex );
			const Str endNode = DBFReadStringAttribute( handle, shape, endNodeIndex );

			if (!nodeMap.contains( startNode ))
			{
				nodeMap[startNode] = nodeMap.size();
			}
			if (!nodeMap.contains( endNode ))
			{
				nodeMap[endNode] = nodeMap.size();
			}

			const Vertex source = nodeMap[startNode];
			const Vertex target = nodeMap[endNode];
			const double weight = (double)length;

			while (result->numVertices() <= source)
			{
				result->addVertex( {} );
			}
			while (result->numVertices() <= target)
			{
				result->addVertex( {} );
			}

			if (!result->edge( source, target ))
			{
				result->addEdge( source, target, { weight } );
			}
		}

		DBFClose( handle );
	}

	return result;
}

FACTORY_BEGIN_JSON( "shapefile", ShapefileParser, GraphParser )

	JSON_ARG( Str, filepath )
	JSON_ARG( Str, directory )

	FACTORY_FABRICATE( std::move( filepath ), std::move( directory ) )

FACTORY_END()