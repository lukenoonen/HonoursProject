#ifndef PATHSOLVERBUILDER_H
#define PATHSOLVERBUILDER_H

#include "PathSolver.h"

class PathSolverBuilder
{
public:
	PathSolverBuilder( const WeightedGraph& graph );

	Ptr<PathSolver> create() const;

protected:
	const WeightedGraph& graph() const;

	virtual Ptr<PathSolver> createInternal() const = 0;

private:
	const WeightedGraph& _graph;
};

FACTORY_CREATE_BASE_JSON( PathSolverBuilder, const WeightedGraph& )

JSON_CREATE_FACTORY( PathSolverBuilder, const WeightedGraph& )

#endif // PATHSOLVERBUILDER_H