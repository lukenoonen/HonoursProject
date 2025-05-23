#include "DijkstraSolverBuilder.hpp"
#include "DijkstraSolver.hpp"

Ptr<PathSolver> DijkstraSolverBuilder::createInternal(const WeightedGraph& graph
) const
{
	return std::make_unique<DijkstraSolver>(graph);
}

FACTORY_BEGIN_JSON("dijkstra_solver", DijkstraSolverBuilder, PathSolverBuilder)

	FACTORY_FABRICATE()

FACTORY_END()