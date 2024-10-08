#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

#include "Graph.h"

class WeightedVertex;
class WeightedEdge;

class WeightedGraph : public Graph<WeightedVertex, WeightedEdge>
{

};

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

#endif // WEIGHTEDGRAPH_H