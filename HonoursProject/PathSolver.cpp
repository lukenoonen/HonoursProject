#include "PathSolver.h"
#include "PathSolverBuilder.h"

JSON_BEGIN( Ptr<PathSolver>, const WeightedGraph& g )

	JSON_ARG_SINGLE( Ptr<PathSolverBuilder>, builder, g )
	
	JSON_FABRICATE( builder->create() )

JSON_END()