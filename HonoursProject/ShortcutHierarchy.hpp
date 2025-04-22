#ifndef SHORTCUTHIERARCHY_H
#define SHORTCUTHIERARCHY_H

#include "Util.hpp"

#include "PathSolver.hpp"
#include "ShortcutGraph.hpp"
#include "WeightedGraph.hpp"

class ShortcutHierarchy : public PathSolver
{
private:
	friend class ShortcutHierarchyAnalyser;

private:
	USING_GRAPH(WeightedGraph);

public:
	constexpr static double DEFAULT_SCALE = 8.0;

public:
	ShortcutHierarchy();
	ShortcutHierarchy(const WeightedGraph& source, double scale);

	void extend(ShortcutGraph layer, const Vec<ShortcutGraph::Vertex>& discard);

	void finalize();

	[[nodiscard]] const ShortcutGraph& top() const;

	[[nodiscard]] size_t vertexLevel(Vertex v) const;
	[[nodiscard]] size_t height() const;

	[[nodiscard]] double distance(
		Vertex s,
		Vertex t
	) const final;

	[[nodiscard]] Vec<double> distances(
		Vertex             s,
		const Vec<Vertex>& ts
	) const final;

public:
	friend void serialize(std::ostream& os, const ShortcutHierarchy& data);
	friend void deserialize(std::istream& is, ShortcutHierarchy& data);

private:
	Vec<ShortcutGraph> _hierarchy;
	Vec<size_t>        _ladder;
	double             _scale;
};

#endif  // SHORTCUTHIERARCHY_H