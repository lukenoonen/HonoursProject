#ifndef WEIGHTEDGRAPH_H
#define WEIGHTEDGRAPH_H

#include "BaseGraph.hpp"
#include "Filters.hpp"

class WeightedVertex;
class WeightedEdge;

class WeightedGraph : public BaseGraph<WeightedVertex, WeightedEdge, NoFilter>
{
public:
	void normalize();

public:
	friend void serialize(std::ostream& os, const WeightedGraph& data);
	friend void deserialize(std::istream& is, WeightedGraph& data);
};

class WeightedVertex
{
public:
	friend void serialize(std::ostream& os, const WeightedVertex& data);
	friend void deserialize(std::istream& is, WeightedVertex& data);
};

class WeightedEdge
{
public:
	WeightedEdge() = default;
	explicit WeightedEdge(double weight);

	void normalize(double by);

	[[nodiscard]] double weight() const;

	friend void serialize(std::ostream& os, const WeightedEdge& data);
	friend void deserialize(std::istream& is, WeightedEdge& data);

private:
	double _weight;
};

#endif  // WEIGHTEDGRAPH_H