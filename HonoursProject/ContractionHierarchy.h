#ifndef CONTRACTIONHIERARCHY_H
#define CONTRACTIONHIERARCHY_H

#include "Util.h"

#include "WeightedGraph.h"
#include "ContractionGraph.h"
#include "PathSolver.h"

class ContractionHierarchy : public PathSolver
{
public:
	ContractionHierarchy() = default;
	ContractionHierarchy( const WeightedGraph& source );

	ContractionGraph::Contraction contract( WeightedGraph::Vertex v );
	void applyContraction( ContractionGraph::Contraction contraction );

	void finalize();

	double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const final;

public:
	friend void serialize( std::ostream& os, const ContractionHierarchy& data );
	friend void deserialize( std::istream& is, ContractionHierarchy& data );

private:
	ContractionGraph _contractionGraph;
};

#endif // CONTRACTIONHIERARCHY_H