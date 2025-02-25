#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

#include "Util.h"

#include "JSON.h"

#include "BaseGraph.h"
#include "Filters.h"

class WeightedVertex;
class WeightedEdge;

class WeightedGraph : public BaseGraph<WeightedVertex, WeightedEdge, NoFilter>
{
public:
	void normalize();
};

class WeightedVertex
{
public:
	friend void serialize( std::ostream& os, const WeightedVertex& data );
	friend void deserialize( std::istream& is, WeightedVertex& data );
};

class WeightedEdge
{
public:
	WeightedEdge() = default;
	WeightedEdge( double weight );

	void normalize( double by );

	double weight() const;

	friend void serialize( std::ostream& os, const WeightedEdge& data );
	friend void deserialize( std::istream& is, WeightedEdge& data );

private:
	double _weight;
};

#endif // WEIGHTEDGRAPH_H