#include "DijkstraSolver.h"
#include "Dijkstra.h"

DijkstraSolver::DijkstraSolver( const WeightedGraph& g )
	: _graph( g )
{

}

double DijkstraSolver::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	const auto searchResults = dijkstraSearch( _graph, s, { t }  );
	return searchResults.distance( t );
}

Vec<double> DijkstraSolver::distances( Vertex s, const Vec<Vertex>& ts ) const
{
	const auto searchResults = dijkstraSearch( _graph, s, ts );
	Vec<double> result;
	result.reserve( ts.size() );
	for (const auto t : ts)
	{
		result.push_back( searchResults.distance( t ) );
	}
	return result;
}
