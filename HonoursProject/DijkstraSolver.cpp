#include "DijkstraSolver.h"
#include "Dijkstra.h"

DijkstraSolver::DijkstraSolver( const WeightedGraph& g )
	: _g( g )
{

}

double DijkstraSolver::distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const
{
	const auto searchResults = dijkstraSearch( _g, s, t );
	return searchResults.distance( t );
}