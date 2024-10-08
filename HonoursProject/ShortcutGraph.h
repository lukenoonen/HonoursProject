#ifndef SHORTCUTGRAPH_H

#include "Graph.h"
#include "WeightedGraph.h"

class ShortcutVertex;
class ShortcutEdge;

class ShortcutGraph : public Graph<ShortcutVertex, ShortcutEdge>
{
public:
	ShortcutGraph( const WeightedGraph& source );
	ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<vertex_descriptor>& discard );
	template <class VertexCopier, class EdgeCopier>
	ShortcutGraph( const ShortcutGraph& other, VertexCopier vc, EdgeCopier ec );

	ShortcutGraph::vertex_descriptor fromSource( WeightedGraph::vertex_descriptor v ) const;
	WeightedGraph::vertex_descriptor toSource( ShortcutGraph::vertex_descriptor v ) const;
};

class ShortcutVertex
{
public:
	ShortcutVertex( WeightedGraph::vertex_descriptor mapped );

	WeightedGraph::vertex_descriptor mapped() const;

private:
	WeightedGraph::vertex_descriptor _mapped;
};

class ShortcutEdge
{
public:
	using path_type = std::vector<const ShortcutEdge*>;

public:
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