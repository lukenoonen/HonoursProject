#ifndef SHORTCUTGRAPH_H
#define SHORTCUTGRAPH_H

#include "Util.h"

#include "BaseGraph.h"
#include "WeightedGraph.h"

#include "Profiler.h"

CREATE_PROFILER_SET( shortcut_graph );

class ShortcutVertex;
class ShortcutEdge;

class ShortcutGraph : public BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>
{
public:
	ShortcutGraph() = default;
	ShortcutGraph( const ShortcutGraph& ) = default;
	ShortcutGraph(
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);
	ShortcutGraph(
		const ShortcutGraph&            prev,
		const Vec<Vertex>&              discard,
		double                          maxEdge,
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);

	ShortcutGraph::Vertex fromSource( WeightedGraph::Vertex v ) const;
	WeightedGraph::Vertex toSource( ShortcutGraph::Vertex v ) const;

private:
	void addSourceEdges(
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);

	void calculateMap();

public:
	friend void serialize( std::ostream& os, const ShortcutGraph& data );
	friend void deserialize( std::istream& is, ShortcutGraph& data );

private:
	Map<Vertex, Vertex> _map;
};

class ShortcutVertex
{
public:
	ShortcutVertex() = default;
	ShortcutVertex( WeightedGraph::Vertex mapped );

	WeightedGraph::Vertex mapped() const;

	friend void serialize( std::ostream& os, const ShortcutVertex& data );
	friend void deserialize( std::istream& is, ShortcutVertex& data );

private:
	WeightedGraph::Vertex _mapped;
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

#endif // SHORTCUTGRAPH_H