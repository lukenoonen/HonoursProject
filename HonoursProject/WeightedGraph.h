#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

#include "BaseGraph.h"

class WeightedVertex;
class WeightedEdge;

class WeightedGraph : public BaseGraph<WeightedVertex, WeightedEdge>
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