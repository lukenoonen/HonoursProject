#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

#include "Graph.h"

class WeightedVertex
{

};

class WeightedEdge
{
public:
	WeightedEdge( double weight );

	double weight() const;

private:
	double _weight;
};

class WeightedGraph : public Graph<WeightedVertex, WeightedEdge>
{

};

#endif // WEIGHTEDGRAPH_H