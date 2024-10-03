#include "WeightedGraph.h"

WeightedEdge::WeightedEdge( double weight )
	: _weight( weight )
{
	
}

double WeightedEdge::weight() const
{
	return _weight;
}