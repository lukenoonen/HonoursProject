#ifndef CONTRACTIONGRAPH_H
#define CONTRACTIONGRAPH_H

#include "Util.h"

#include "BaseContractionGraph.h"

class ContractionVertex;

class ContractionGraph : public BaseContractionGraph<ContractionVertex>
{
public:
	ContractionGraph() = default;
	ContractionGraph( const WeightedGraph& source );

	void applyContraction( Contraction contraction );
	void finalize();

public:
	friend void serialize( std::ostream& os, const ContractionGraph& data );
	friend void deserialize( std::istream& is, ContractionGraph& data );
};

class ContractionVertex
{
public:
	ContractionVertex() = default;

	void setImportance( size_t importance );

	size_t importance() const;

	friend void serialize( std::ostream& os, const ContractionVertex& data );
	friend void deserialize( std::istream& is, ContractionVertex& data );

private:
	size_t _importance;
};

#endif // CONTRACTIONGRAPH_H