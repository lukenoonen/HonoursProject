#ifndef CONTRACTIONHIERARCHY_H
#define CONTRACTIONHIERARCHY_H

#include "Util.h"

#include "WeightedGraph.h"
#include "ContractionGraph.h"
#include "PathSolver.h"

class ContractionHierarchy : public PathSolver
{
private:
	USING_GRAPH( WeightedGraph );

public:
	ContractionHierarchy() = default;

	void set( ContractionGraph graph );

	double distance( Vertex s, Vertex t ) const final;
	Vec<double> distances( Vertex s, const Vec<Vertex>& ts ) const final;

public:
	friend void serialize( std::ostream& os, const ContractionHierarchy& data );
	friend void deserialize( std::istream& is, ContractionHierarchy& data );

private:
	ContractionGraph _contractionGraph;
};

#endif // CONTRACTIONHIERARCHY_H