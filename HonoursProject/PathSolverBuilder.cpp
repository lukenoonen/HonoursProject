#include "PathSolverBuilder.hpp"

Ptr<PathSolver> PathSolverBuilder::create(const WeightedGraph& graph) const
{
	return createInternal(graph);
}

void PathSolverBuilder::clearCache() const
{
}

JSON_DEFINE_FACTORY(PathSolverBuilder)