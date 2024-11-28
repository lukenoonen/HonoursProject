#ifndef SHORTCUTGRAPH_H
#define SHORTCUTGRAPH_H

#include "BaseGraph.h"
#include "WeightedGraph.h"

class ShortcutVertex;
class ShortcutEdge;

class ShortcutGraph : public BaseGraph<ShortcutVertex, ShortcutEdge>
{
public:
	ShortcutGraph( const WeightedGraph& source );
	ShortcutGraph( const ShortcutGraph& prev, const std::vector<VertexDescriptor>& discard );

	ShortcutGraph::VertexDescriptor fromSource( WeightedGraph::VertexDescriptor v ) const;
	WeightedGraph::VertexDescriptor toSource( ShortcutGraph::VertexDescriptor v ) const;

private:
	void calculateMap();

private:
	std::unordered_map<WeightedGraph::VertexDescriptor, ShortcutGraph::VertexDescriptor> _map;
};

class ShortcutVertex
{
public:
	ShortcutVertex() = default;
	ShortcutVertex( WeightedGraph::VertexDescriptor mapped );

	WeightedGraph::VertexDescriptor mapped() const;

private:
	WeightedGraph::VertexDescriptor _mapped;
};

class ShortcutEdge
{
public:
	using PathType = std::vector<const ShortcutEdge*>;

public:
	ShortcutEdge() = default;
	ShortcutEdge( double weight );
	ShortcutEdge( const ShortcutEdge* prev );
	ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second );

	const PathType& path() const;
	double weight() const;
	double maxEdge() const;

private:
	PathType _path;

	double _weight;
	double _maxEdge;
};

#endif // SHORTCUTGRAPH_H