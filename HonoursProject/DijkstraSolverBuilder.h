#ifndef DIJKSTRASOLVERBUILDER_H
#define DIJKSTRASOLVERBUILDER_H

#include "PathSolverBuilder.h"

class DijkstraSolverBuilder : public PathSolverBuilder
{
public:
	DijkstraSolverBuilder( const WeightedGraph& graph );

protected:
	Ptr<PathSolver> createInternal() const final;
};

FACTORY_CREATE_JSON( DijkstraSolverBuilder, PathSolverBuilder, const WeightedGraph& )

#endif // DIJKSTRASOLVERBUILDER_H