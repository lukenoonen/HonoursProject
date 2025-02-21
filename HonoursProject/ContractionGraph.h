#ifndef CONTRACTIONGRAPH_H
#define CONTRACTIONGRAPH_H

#include "BaseGraph.h"
#include "WeightedGraph.h"

class ContractionVertex;
class ContractionEdge;

class ContractionGraph : public BaseGraph<ContractionVertex, ContractionEdge, VertexFilter>
{
public:
	class Contraction;

public:
	ContractionGraph() = default;
	ContractionGraph( const WeightedGraph& source );

	Contraction contract( ContractionGraph::Vertex v );
	void applyContraction( Contraction contraction );
	void finalize();

public:
	friend void serialize( std::ostream& os, const ContractionGraph& data );
	friend void deserialize( std::istream& is, ContractionGraph& data );
};

class ContractionGraph::Contraction
{
public:
	using Shortcut = std::tuple<Vertex, Vertex, ContractionEdge>;

public:
	Contraction() = default;
	Contraction(
		Vertex        contracted,
		Vec<Shortcut> shortcuts,
		size_t        edgeCount
	);

	int edgeDifference() const;

	void apply( ContractionGraph& graph );

private:
	Vertex        _contracted;
	Vec<Shortcut> _shortcuts;
	int           _edgeDifference;
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

class ContractionEdge
{
public:
	using PathType = Vec<WeightedGraph::Edge>;

public:
	ContractionEdge() = default;
	ContractionEdge( WeightedGraph::Edge e, double weight );
	ContractionEdge( const ContractionEdge& first, const ContractionEdge& second );

	const PathType& path() const;
	double weight() const;

	friend void serialize( std::ostream& os, const ContractionEdge& data );
	friend void deserialize( std::istream& is, ContractionEdge& data );

private:
	PathType _path;
	double   _weight;
};

#endif // CONTRACTIONGRAPH_H