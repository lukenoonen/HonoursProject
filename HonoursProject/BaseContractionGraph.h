#ifndef BASECONTRACTIONGRAPH_H
#define BASECONTRACTIONGRAPH_H

#include "Util.h"

#include "BaseGraph.h"
#include "WeightedGraph.h"

class ShortcutEdge;

template <class V>
class BaseContractionGraph : public BaseGraph<V, ShortcutEdge, VertexFilter>
{
public:
	class Contraction;

public:
	size_t currentImportance() const;
	Contraction contract( BaseContractionGraph<V>::Vertex v );
	void applyContraction( Contraction contraction );
};

template <class V>
class BaseContractionGraph<V>::Contraction
{
private:
	friend class BaseContractionGraph<V>;

private:
	using Vertex = typename BaseContractionGraph<V>::Vertex;

public:
	using Shortcut = std::tuple<Vertex, Vertex, ShortcutEdge>;

public:
	Contraction() = default;
	Contraction(
		Vertex        contracted,
		size_t        edgeCount,
		Vec<Shortcut> shortcuts
	);

	Vertex contracted() const;
	int edgeDifference() const;

private:
	Vertex        _contracted;
	int           _edgeDifference;
	Vec<Shortcut> _shortcuts;
};

class ShortcutEdge
{
public:
	using PathType = Vec<WeightedGraph::Edge>;

public:
	ShortcutEdge() = default;
	ShortcutEdge( WeightedGraph::Edge e, double weight );
	ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second );

	const PathType& path() const;
	double weight() const;

	friend void serialize( std::ostream& os, const ShortcutEdge& data );
	friend void deserialize( std::istream& is, ShortcutEdge& data );

private:
	PathType _path;
	double   _weight;
};

#include "BaseContractionGraph.inl"

#endif // BASECONTRACTIONGRAPH_H