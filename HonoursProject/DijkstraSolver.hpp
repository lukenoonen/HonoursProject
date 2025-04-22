#ifndef DIJKSTRASOLVER_H
#define DIJKSTRASOLVER_H

#include "Util.hpp"

#include "PathSolver.hpp"

class DijkstraSolver : public PathSolver
{
private:
	USING_GRAPH(WeightedGraph);

public:
	explicit DijkstraSolver(const WeightedGraph& g);

	[[nodiscard]] double      distance(Vertex s, Vertex t) const final;
	[[nodiscard]] Vec<double> distances(Vertex s, const Vec<Vertex>& ts) const final;

private:
	const WeightedGraph* _graph;
};

#endif  // DIJKSTRASOLVER_H