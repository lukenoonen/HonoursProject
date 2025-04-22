#ifndef BASECONTRACTIONGRAPH_H
#define BASECONTRACTIONGRAPH_H

#include "Util.hpp"

#include "BaseGraph.hpp"
#include "WeightedGraph.hpp"

class ShortcutEdge;

template <class V>
class BaseContractionGraph : public BaseGraph<V, ShortcutEdge, VertexFilter>
{
public:
	class Contraction;

public:
	BaseContractionGraph()                                       = default;
	virtual ~BaseContractionGraph()                              = default;
	BaseContractionGraph(const BaseContractionGraph&)            = default;
	BaseContractionGraph& operator=(const BaseContractionGraph&) = default;
	BaseContractionGraph(BaseContractionGraph&&)                 = default;
	BaseContractionGraph& operator=(BaseContractionGraph&&)      = default;

	[[nodiscard]] size_t currentImportance() const;

	Contraction getContraction(typename BaseContractionGraph<V>::Vertex v);

	void applyContraction(Contraction contraction);
};

template <class V>
class BaseContractionGraph<V>::Contraction
{
private:
	friend class BaseContractionGraph<V>;

private:
	using Vertex = typename BaseContractionGraph<V>::Vertex;

public:
	using Shortcut = std::tuple<Vertex, Vertex, ShortcutEdge>;

public:
	Contraction() = default;
	Contraction(Vertex contracted, size_t edgeCount, Vec<Shortcut> shortcuts);

	[[nodiscard]] Vertex contractedVertex() const;
	[[nodiscard]] int    edgeDifference() const;

private:
	Vertex        _contractedVertex;
	int           _edgeDifference;
	Vec<Shortcut> _shortcuts;
};

class ShortcutEdge
{
public:
	using PathType = Vec<WeightedGraph::Edge>;

public:
	ShortcutEdge() = default;
	ShortcutEdge(WeightedGraph::Edge e, double weight);
	ShortcutEdge(const ShortcutEdge& first, const ShortcutEdge& second);

	[[nodiscard]] const PathType& path() const;
	[[nodiscard]] double          weight() const;

	friend void serialize(std::ostream& os, const ShortcutEdge& data);
	friend void deserialize(std::istream& is, ShortcutEdge& data);

private:
	PathType _path;
	double   _weight;
};

#include "BaseContractionGraph.inl"

#endif  // BASECONTRACTIONGRAPH_H