#ifndef SHORTCUTHIERARCHY_H
#define SHORTCUTHIERARCHY_H

#include "Util.h"

#include "WeightedGraph.h"
#include "ShortcutGraph.h"
#include "PathSolver.h"
#include "Serialize.h"
#include "Factory.h"

class ShortcutHierarchy : public PathSolver
{
public:
	ShortcutHierarchy() = default;

	ShortcutHierarchy( const WeightedGraph& g );

	void extend( const Vec<ShortcutGraph::Vertex>& discard );

	const ShortcutGraph& top() const;

	double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const final;

public:
	friend void serialize( std::ostream& os, const ShortcutHierarchy& data );
	friend void deserialize( std::istream& is, ShortcutHierarchy& data );

private:
	Vec<ShortcutGraph> _hierarchy;
	Vec<size_t>        _ladder;
};

// FACTORY_CREATE_CACHE( ShortcutHierarchy, PathSolver )

#endif // SHORTCUTHIERARCHY_H