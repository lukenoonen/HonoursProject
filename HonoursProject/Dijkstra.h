#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Util.h"

#include <boost/heap/fibonacci_heap.hpp>

template <class Graph>
class DijkstraResult
{
public:
	using Vertex = typename Graph::Vertex;
	using Edge   = typename Graph::Edge;

public:
	DijkstraResult() = default;
	DijkstraResult( Vertex source );
	DijkstraResult(
		Vertex vertex,
		Vertex prev,
		Edge   edge,
		double distance
	);

	void update( Vertex prev, Edge edge, double distance );

	bool operator<( const DijkstraResult& other ) const;

	Vertex vertex() const;
	Vertex prev() const;
	Edge   edge() const;
	double distance() const;

	bool isOrigin() const;

private:
	Vertex _vertex;
	Vertex _prev;
	Edge   _edge;
	double _distance;
};

template <class Graph, template <class> class Result>
class ShortestPaths
{
public:
	using Vertex = typename Graph::Vertex;

public:
	ShortestPaths( Vertex to );

	const Result<Graph>& insert( Vertex v, const Result<Graph>& result );

	bool contains( Vertex v ) const;
	double distance( Vertex from ) const;
	size_t size() const;

	void filter( Vertex vertex );

	template <class P>
	bool vertexMap( P predicate ) const;

	template <class P>
	bool pathMap( Vertex from, P predicate ) const;

private:
	Vertex _to;
	Map<Vertex, Result<Graph>> _results;
	Set<Vertex> _filter;
};

template <class Graph, template <class> class Result>
class DijkstraData
{
private:
	using Vertex = typename Graph::Vertex;
	using Edge = typename Graph::Edge;

	using Heap = boost::heap::fibonacci_heap<Result<Graph>>;
	using HeapHandle = DijkstraData<Graph, Result>::Heap::handle_type;
	using HandleMap = Map<Vertex, HeapHandle>;
	using DistanceMap = Map<Vertex, double>;

public:
	template <class... Ts>
	DijkstraData( Vertex to, Ts... args );

	template <class... Ts>
	void decrease( Vertex v, Vertex p, Edge e, double distance, Ts... args );

	const Result<Graph>& extract();

	bool closed( Vertex v ) const;
	double distance( Vertex v ) const;
	bool empty() const;

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
	const Graph& graph,
	typename Graph::Vertex source,
	P                      predicate
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
bool usefulEdge(
	const Graph&         graph,
	typename Graph::Edge e
);

template <class Graph>
ShortestPaths<Graph, DijkstraResult> dijkstraSearch(
	const Graph& graph,
	typename Graph::Vertex source,
	typename Graph::Vertex target
);

#include "Dijkstra.inl"

#endif // DIJKSTRA_H