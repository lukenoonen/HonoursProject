#ifndef PATHSOLVER_H
#define PATHSOLVER_H

#include "Util.hpp"

#include "JSON.hpp"

#include "WeightedGraph.hpp"

class PathSolver
{
private:
	USING_GRAPH(WeightedGraph);

public:
	PathSolver()                             = default;
	virtual ~PathSolver()                    = default;
	PathSolver(const PathSolver&)            = default;
	PathSolver& operator=(const PathSolver&) = default;
	PathSolver(PathSolver&&)                 = default;
	PathSolver& operator=(PathSolver&&)      = default;

public:
	[[nodiscard]] virtual double distance(
		Vertex s,
		Vertex t
	) const = 0;
	
	[[nodiscard]] virtual Vec<double> distances(
		Vertex             s,
		const Vec<Vertex>& ts
	) const = 0;
};

JSON_CREATE(Ptr<PathSolver>)

#endif  // PATHSOLVER_H