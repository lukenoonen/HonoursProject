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
	ShortcutGraph( const ShortcutGraph& source );
	ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<VertexDescriptor>& discard );

	ShortcutGraph::VertexDescriptor fromSource( WeightedGraph::VertexDescriptor v ) const;
	WeightedGraph::VertexDescriptor toSource( ShortcutGraph::VertexDescriptor v ) const;
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
	using path_type = std::vector<const ShortcutEdge*>;

public:
	ShortcutEdge() = default;
	ShortcutEdge( double weight );
	ShortcutEdge( const ShortcutEdge* prev );
	ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second );

	const path_type& path() const;
	double weight() const;
	double maxEdge() const;

private:
	path_type _path;

	double _weight;
	double _maxEdge;
};

#endif // SHORTCUTGRAPH_H