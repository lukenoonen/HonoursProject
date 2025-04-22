#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "BaseGraph.hpp"
#include "Util.hpp"

#include <boost/heap/fibonacci_heap.hpp>

template <class Graph>
class DijkstraResult
{
private:
	USING_GRAPH(Graph);

public:
	DijkstraResult() = default;
	explicit DijkstraResult(Vertex source);
	DijkstraResult(Vertex vertex, Vertex prev, Edge edge, double distance);

	void update(Vertex prev, Edge edge, double distance);

	bool operator<(const DijkstraResult& other) const;

	[[nodiscard]] Vertex vertex() const;
	[[nodiscard]] Vertex prev() const;
	[[nodiscard]] Edge   edge() const;
	[[nodiscard]] double distance() const;

	[[nodiscard]] bool isOrigin() const;

private:
	Vertex _vertex;
	Vertex _prev;
	Edge   _edge;
	double _distance;
};

template <class Graph, template <class> class Result>
class ShortestPaths
{
private:
	USING_GRAPH(Graph);

public:
	explicit ShortestPaths(Vertex to);

	const Result<Graph>& insert(Vertex v, const Result<Graph>& result);

	[[nodiscard]] bool   contains(Vertex v) const;
	[[nodiscard]] double distance(Vertex from) const;
	[[nodiscard]] size_t size() const;

	void filter(Vertex vertex);

	template <class P>
	bool vertexMap(P predicate) const;

	template <class P>
	bool pathMap(Vertex from, P predicate) const;

private:
	Vertex                     _to;
	Map<Vertex, Result<Graph>> _results;
	Set<Vertex>                _filter;
};

template <class Graph, template <class> class Result>
class DijkstraData
{
private:
	USING_GRAPH(Graph);

	using Heap        = boost::heap::fibonacci_heap<Result<Graph>>;
	using HeapHandle  = DijkstraData<Graph, Result>::Heap::handle_type;
	using HandleMap   = Map<Vertex, HeapHandle>;
	using DistanceMap = Map<Vertex, double>;

public:
	template <class... Ts>
	explicit DijkstraData(Vertex to, Ts... args);

	template <class... Ts>
	void decreasePriority(
		Vertex v,
		Vertex p,
		Edge   e,
		double distance,
		Ts... args
	);

	const Result<Graph>& extractMin();

	[[nodiscard]] bool   closed(Vertex v) const;
	[[nodiscard]] double distance(Vertex v) const;
	[[nodiscard]] bool   empty() const;

	ShortestPaths<Graph, Result> results();

private:
	HandleMap   _handles;
	DistanceMap _distances;

	Heap _queue;

	ShortestPaths<Graph, Result> _results;
};

enum class PredicateResponse : unsigned char
{
	FALSE,
	TRUE,
	BREAK
};

template <class Graph, template <class> class Result, class P>
ShortestPaths<Graph, Result> dijkstra(
	const Graph&           graph,
	typename Graph::Vertex source,
	P                      predicate
);

template <class Graph, template <class> class Result>
inline ShortestPaths<Graph, Result> dijkstra(
	const Graph&           graph,
	typename Graph::Vertex source
);

template <class Graph>
bool witnessSearch(
	const Graph&           graph,
	typename Graph::Vertex source,
	typename Graph::Vertex target,
	double                 maxDist
);

template <class Graph>
ShortestPaths<Graph, DijkstraResult> shortestPaths(
	const Graph&           graph,
	typename Graph::Vertex source,
	double                 maxDist,
	double                 minDist
);

template <class Graph>
bool usefulEdge(const Graph& graph, typename Graph::Edge e);

template <class Graph>
ShortestPaths<Graph, DijkstraResult> dijkstraSearch(
	const Graph&                       graph,
	typename Graph::Vertex             source,
	const Vec<typename Graph::Vertex>& targets
);

#include "Dijkstra.inl"

#endif  // DIJKSTRA_H