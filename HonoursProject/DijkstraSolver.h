#ifndef DIJKSTRASOLVER_H
#define DIJKSTRASOLVER_H

#include "Util.h"

#include "PathSolver.h"

class DijkstraSolver : public PathSolver
{
private:
	USING_GRAPH( WeightedGraph );

public:
	DijkstraSolver( const WeightedGraph& g );

	double distance( Vertex s, Vertex t ) const final;
	Vec<double> distances( Vertex s, const Vec<Vertex>& ts ) const final;
	
private:
	const WeightedGraph& _graph;
};

#endif // DIJKSTRASOLVER_H