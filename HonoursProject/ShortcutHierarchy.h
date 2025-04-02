#ifndef SHORTCUTHIERARCHY_H
#define SHORTCUTHIERARCHY_H

#include "Util.h"

#include "WeightedGraph.h"
#include "ShortcutGraph.h"
#include "PathSolver.h"
#include "Serialize.h"

class ShortcutHierarchy : public PathSolver
{
private:
	friend class ShortcutHierarchyAnalyser;

private:
	USING_GRAPH( WeightedGraph );

public:
	ShortcutHierarchy() = default;
	ShortcutHierarchy( const WeightedGraph& source, double scale );

	void extend( ShortcutGraph layer, const Vec<ShortcutGraph::Vertex>& discard );

	const ShortcutGraph& top() const;

	void finalize();

	size_t vertexLevel( Vertex v ) const;
	size_t height() const;

	double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const final;
	Vec<double> distances( Vertex s, const Vec<Vertex>& ts ) const final;

public:
	friend void serialize( std::ostream& os, const ShortcutHierarchy& data );
	friend void deserialize( std::istream& is, ShortcutHierarchy& data );

private:
	Vec<ShortcutGraph> _hierarchy;
	Vec<size_t>        _ladder;
	double             _scale;
};

#endif // SHORTCUTHIERARCHY_H