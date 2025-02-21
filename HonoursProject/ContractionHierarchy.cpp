#include "ContractionHierarchy.h"
#include "Dijkstra.h"

namespace
{
	bool dijkstraStep(
		const ContractionGraph&                         graph,
		DijkstraData<ContractionGraph, DijkstraResult>& first,
		DijkstraData<ContractionGraph, DijkstraResult>& second,
		double&                                         bestDistance
	)
	{
		const auto& ex = first.extract();
		const auto u   = ex.vertex();
		graph.edgeMap( ex.vertex(), [&ex, &graph, u, &first, &second, &bestDistance]( const auto e ) {
			const auto v = graph.other( e, u );
			if (first.closed( v )) { return false; }
			if (graph[v].importance() < graph[u].importance()) { return false; }
			const double vDist = first.distance( v );
			const double newDist = ex.distance() + graph[e].weight();
			if (newDist >= vDist) { return false; }
			first.decrease( v, ex.vertex(), e, newDist );
			const double distance = newDist + second.distance( v );
			if (bestDistance > distance)
			{
				bestDistance = distance;
			}
			return false;
		} );
		
		return !first.empty() && ex.distance() < bestDistance;
	}
}

ContractionHierarchy::ContractionHierarchy( const WeightedGraph& source )
	: _contractionGraph( source )
{

}

ContractionGraph::Contraction ContractionHierarchy::contract( WeightedGraph::Vertex v )
{
	return _contractionGraph.contract( v );
}

void ContractionHierarchy::applyContraction( ContractionGraph::Contraction contraction )
{
	_contractionGraph.applyContraction( std::move( contraction ) );
}

void ContractionHierarchy::finalize()
{
	_contractionGraph.finalize();
}

double ContractionHierarchy::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	if (s == t) { return 0.0; }

	DijkstraData<ContractionGraph, DijkstraResult> forward( s );
	DijkstraData<ContractionGraph, DijkstraResult> backward( t );

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch = true;
	bool backwardSearch = true;
	
	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = dijkstraStep( _contractionGraph, forward, backward, bestDistance );
		}
		if (backwardSearch)
		{
			backwardSearch = dijkstraStep( _contractionGraph, backward, forward, bestDistance );
		}
	}

	return bestDistance;
}

void serialize( std::ostream& os, const ContractionHierarchy& data )
{
	serialize( os, data._contractionGraph );
}

void deserialize( std::istream& is, ContractionHierarchy& data )
{
	deserialize( is, data._contractionGraph );
}