#ifndef SHORTCUTGRAPH_H

#include "Graph.h"

class ShortcutVertex;
class ShortcutEdge;

class ShortcutGraph : public Graph<ShortcutVertex, ShortcutEdge>
{
public:
	ShortcutGraph( const ShortcutGraph& gPrev, const std::vector<vertex_descriptor>& c );
};

class ShortcutVertex
{
public:
	ShortcutVertex( ShortcutGraph::vertex_descriptor mapped );

	ShortcutGraph::vertex_descriptor mapped() const;

private:
	ShortcutGraph::vertex_descriptor _mapped;
};

class ShortcutEdge
{
public:
	ShortcutEdge( std::vector<ShortcutEdge*> path );
	ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second );

	const std::vector<ShortcutEdge*>& path() const;
	double weight() const;
	double maxEdge() const;

private:
	std::vector<ShortcutEdge*> _path;
	double _weight;
	double _maxEdge;
};

#endif // SHORTCUTGRAPH_H