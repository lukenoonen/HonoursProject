#include "ShortcutGraph.hpp"

ShortcutGraph::ShortcutGraph(
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
{
	source.vertexMap(
		[&source, this](const auto v) {
			addVertex(ShortcutVertex{v});
			return false;
		}
	);
	calculateMap();
	addSourceEdges(source, edges, edgeRange);
}

ShortcutGraph::ShortcutGraph(
	const ShortcutGraph&            prev,
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
	: ShortcutGraph(prev)
{
	addSourceEdges(source, edges, edgeRange);
}

void ShortcutGraph::finalize()
{
	const Set<Vertex> discard = filter().set();
	filter().clear();
	removeVertices(discard);
	calculateMap();
}

ShortcutGraph::Vertex ShortcutGraph::fromSource(WeightedGraph::Vertex v) const
{
	return _map.find(v)->second;
}

WeightedGraph::Vertex ShortcutGraph::toSource(ShortcutGraph::Vertex v) const
{
	return get(v).mapped();
}

void ShortcutGraph::addSourceEdges(
	const WeightedGraph&            source,
	const Vec<WeightedGraph::Edge>& edges,
	const Pair<size_t, size_t>&     edgeRange
)
{
	for (size_t i = edgeRange.first; i < edgeRange.second; i++)
	{
		const auto& e = edges[i];
		const auto  u = fromSource(source.source(e));
		const auto  v = fromSource(source.target(e));
		addEdge(u, v, {e, source[e].weight()});
	}
}

void ShortcutGraph::calculateMap()
{
	_map.clear();
	vertexMap(
		[this](const auto v) {
			_map[get(v).mapped()] = v;
			return false;
		}
	);
}

void serialize(std::ostream& os, const ShortcutGraph& data)
{
	using BaseType = BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>;
	serialize(os, data._map);
	serialize(os, static_cast<const BaseType&>(data));
}

void deserialize(std::istream& is, ShortcutGraph& data)
{
	using BaseType = BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>;
	deserialize(is, data._map);
	deserialize(is, static_cast<BaseType&>(data));
}

ShortcutVertex::ShortcutVertex(WeightedGraph::Vertex mapped)
	: _mapped(mapped)
{
}

WeightedGraph::Vertex ShortcutVertex::mapped() const
{
	return _mapped;
}

void serialize(std::ostream& os, const ShortcutVertex& data)
{
	serialize(os, data._mapped);
}

void deserialize(std::istream& is, ShortcutVertex& data)
{
	deserialize(is, data._mapped);
}