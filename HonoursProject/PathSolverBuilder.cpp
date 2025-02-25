#include "PathSolverBuilder.h"

Ptr<PathSolver> PathSolverBuilder::create( const WeightedGraph& graph ) const
{
	return createInternal( graph );
}

JSON_DEFINE_FACTORY( PathSolverBuilder )