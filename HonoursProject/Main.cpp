#include "WeightedGraph.h"
#include "GraphParser.h"
#include "RoadDParser.h"
#include "CIHierarchy.h"

#include <iostream>
#include <random>

double random()
{
	static std::random_device rd;
	static std::mt19937 generator( rd() );
	static std::uniform_real_distribution<> distribution( 0.0, 1.0 );
	return distribution( generator );
}

int main( int argc, const char* argv[] )
{
	for (int i = 1; i < argc; i++)
	{
		RoadDParser parser( argv[i] );
		std::optional<WeightedGraph> graph = parser.create();
		if (graph)
		{
			CIHierarchy hierarchy( *graph );
		}
	}

	/*
	const size_t max = 500;
	for (size_t i = 0; i < max; i++)
	{
		WeightedGraph graph;
		for (size_t v = 0; v < max; v++)
		{
			graph.addVertex( {} );
		}

		for (size_t v = 0; v < max; v++)
		{
			for (size_t w = v + 1; w < max; w++)
			{
				if (random() < 0.5)
				{
					graph.addEdge( v, w, { 1.0 } );
				}
			}
		}

		std::vector<WeightedGraph::VertexDescriptor> remove;
		remove.reserve( max );

		for (size_t v = 0; v < max; v++)
		{
			if (random() < 0.8)
			{
				remove.push_back( v );
			}
		}

		graph.removeVertices( remove );
	}*/

	return 0;
}