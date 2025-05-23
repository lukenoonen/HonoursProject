#ifndef DIJKSTRASOLVERBUILDER_H
#define DIJKSTRASOLVERBUILDER_H

#include "Util.hpp"

#include "PathSolverBuilder.hpp"

class DijkstraSolverBuilder : public PathSolverBuilder
{
protected:
	Ptr<PathSolver> createInternal(const WeightedGraph& graph) const final;
};

FACTORY_CREATE_JSON(DijkstraSolverBuilder, PathSolverBuilder)

#endif  // DIJKSTRASOLVERBUILDER_H