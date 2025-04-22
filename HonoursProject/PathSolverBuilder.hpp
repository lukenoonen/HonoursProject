#ifndef PATHSOLVERBUILDER_H
#define PATHSOLVERBUILDER_H

#include "Util.hpp"

#include "Factory.hpp"

#include "PathSolver.hpp"
#include "WeightedGraph.hpp"

class PathSolverBuilder
{
public:
	PathSolverBuilder()                                    = default;
	virtual ~PathSolverBuilder()                           = default;
	PathSolverBuilder(const PathSolverBuilder&)            = default;
	PathSolverBuilder& operator=(const PathSolverBuilder&) = default;
	PathSolverBuilder(PathSolverBuilder&&)                 = default;
	PathSolverBuilder& operator=(PathSolverBuilder&&)      = default;

	[[nodiscard]] Ptr<PathSolver> create(const WeightedGraph& graph) const;

protected:
	[[nodiscard]] virtual Ptr<PathSolver> createInternal(
		const WeightedGraph& graph
	) const = 0;
};

FACTORY_CREATE_BASE_JSON(PathSolverBuilder)

JSON_CREATE_FACTORY(PathSolverBuilder)

#endif  // PATHSOLVERBUILDER_H