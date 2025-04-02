#ifndef PATHSOLVER_H
#define PATHSOLVER_H

#include "Util.h"

#include "JSON.h"

#include "WeightedGraph.h"
#include "Factory.h"

#include "Logger.h"

class PathSolver
{
private:
	USING_GRAPH( WeightedGraph );

public:
	virtual ~PathSolver() = default;

public:
	virtual double distance( Vertex s, Vertex t ) const = 0;
	virtual Vec<double> distances( Vertex s, const Vec<Vertex>& ts ) const = 0;
};

JSON_CREATE( Ptr<PathSolver> )

#endif // PATHSOLVER_H