#ifndef PATHSOLVER_H
#define PATHSOLVER_H

#include "Util.h"

#include "JSON.h"

#include "WeightedGraph.h"
#include "Factory.h"

class PathSolver
{
public:
	virtual double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const = 0;
};

JSON_CREATE( Ptr<PathSolver>, const WeightedGraph& )

#endif // PATHSOLVER_H