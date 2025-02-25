#ifndef PATHSOLVERBUILDER_H
#define PATHSOLVERBUILDER_H

#include "Util.h"

#include "PathSolver.h"
#include "WeightedGraph.h"

class PathSolverBuilder
{
public:
	Ptr<PathSolver> create( const WeightedGraph& graph ) const;

protected:
	virtual Ptr<PathSolver> createInternal( const WeightedGraph& graph ) const = 0;
};

FACTORY_CREATE_BASE_JSON( PathSolverBuilder )

JSON_CREATE_FACTORY( PathSolverBuilder )

#endif // PATHSOLVERBUILDER_H