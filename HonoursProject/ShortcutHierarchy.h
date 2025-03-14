#ifndef SHORTCUTHIERARCHY_H
#define SHORTCUTHIERARCHY_H

#include "Util.h"

#include "WeightedGraph.h"
#include "ShortcutGraph.h"
#include "PathSolver.h"
#include "Serialize.h"

class ShortcutHierarchy : public PathSolver
{
public:
	ShortcutHierarchy() = default;
	ShortcutHierarchy(
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);

	void extend(
		const Vec<ShortcutGraph::Vertex>& discard,
		const WeightedGraph&              source,
		const Vec<WeightedGraph::Edge>&   edges,
		const Pair<size_t, size_t>&       edgeRange
	);

	ShortcutGraph::Contraction contract( ShortcutGraph::Vertex v );
	void applyContraction( ShortcutGraph::Contraction contraction );
	void finalizeLayer();

	void finalize();

	ShortcutGraph& top();

	size_t level( ShortcutGraph::Vertex v ) const;
	size_t levels() const;

	double distance( WeightedGraph::Vertex s, WeightedGraph::Vertex t ) const final;

public:
	friend void serialize( std::ostream& os, const ShortcutHierarchy& data );
	friend void deserialize( std::istream& is, ShortcutHierarchy& data );

private:
	Vec<ShortcutGraph> _hierarchy;
	Vec<size_t>        _ladder;
};

#endif // SHORTCUTHIERARCHY_H