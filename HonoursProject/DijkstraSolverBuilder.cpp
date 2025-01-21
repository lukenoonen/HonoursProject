#include "DijkstraSolverBuilder.h"
#include "DijkstraSolver.h"

DijkstraSolverBuilder::DijkstraSolverBuilder( const WeightedGraph& graph )
	: PathSolverBuilder( graph )
{

}

Ptr<PathSolver> DijkstraSolverBuilder::createInternal() const
{
	return std::make_unique<DijkstraSolver>( graph() );
}

FACTORY_BEGIN_JSON_PARAM( "dijkstra_solver", DijkstraSolverBuilder, PathSolverBuilder, (const WeightedGraph&), (const WeightedGraph& g) )

	FACTORY_FABRICATE( g )

FACTORY_END()