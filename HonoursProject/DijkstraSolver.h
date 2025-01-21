#ifndef DIJKSTRASOLVER_H
#define DIJKSTRASOLVER_H

#include "PathSolver.h"

class DijkstraSolver : public PathSolver
{
public:
	DijkstraSolver( const WeightedGraph& g );

	double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const final;
	
private:
	const WeightedGraph& _g;

};

#endif // DIJKSTRASOLVER_H