#include "PathSolverBuilder.h"

PathSolverBuilder::PathSolverBuilder( const WeightedGraph& graph )
	: _graph( graph )
{

}

Ptr<PathSolver> PathSolverBuilder::create() const
{
	return createInternal();
}

const WeightedGraph& PathSolverBuilder::graph() const
{
	return _graph;
}

JSON_DEFINE_FACTORY_PARAM( PathSolverBuilder, (const WeightedGraph&), (const WeightedGraph& g), (g) )