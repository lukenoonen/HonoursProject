#ifndef SHORTCUTGRAPH_H
#define SHORTCUTGRAPH_H

#include "Util.hpp"

#include "BaseContractionGraph.hpp"
#include "WeightedGraph.hpp"

class ShortcutVertex;

class ShortcutGraph : public BaseContractionGraph<ShortcutVertex>
{
public:
	ShortcutGraph() = default;
	ShortcutGraph(
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);
	ShortcutGraph(
		const ShortcutGraph&            prev,
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);

	~ShortcutGraph() override                      = default;
	ShortcutGraph(const ShortcutGraph&)            = default;
	ShortcutGraph& operator=(const ShortcutGraph&) = default;
	ShortcutGraph(ShortcutGraph&&)                 = default;
	ShortcutGraph& operator=(ShortcutGraph&&)      = default;

	void finalize();

	ShortcutGraph::Vertex fromSource(WeightedGraph::Vertex v) const;
	WeightedGraph::Vertex toSource(ShortcutGraph::Vertex v) const;

private:
	void addSourceEdges(
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		const Pair<size_t, size_t>&     edgeRange
	);

	void calculateMap();

public:
	friend void serialize(std::ostream& os, const ShortcutGraph& data);
	friend void deserialize(std::istream& is, ShortcutGraph& data);

private:
	Map<Vertex, Vertex> _map;
};

class ShortcutVertex
{
public:
	ShortcutVertex() = default;
	explicit ShortcutVertex(WeightedGraph::Vertex mapped);

	[[nodiscard]] WeightedGraph::Vertex mapped() const;

	friend void serialize(std::ostream& os, const ShortcutVertex& data);
	friend void deserialize(std::istream& is, ShortcutVertex& data);

private:
	WeightedGraph::Vertex _mapped;
};

#endif  // SHORTCUTGRAPH_H