#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Util.h"

#include "ShortcutGraph.h"

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

template <class Graph>
class ShortestPaths
{
public:
	using Vertex = typename Graph::Vertex;

public:
	ShortestPaths( Vertex to );

	void insert( Vertex v, const DijkstraResult<Graph>& result );

	double distance( Vertex from ) const;

	void filter( Vertex vertex );

	template <class P>
	bool vertexMap( P predicate ) const;

	template <class P>
	bool pathMap( Vertex from, P predicate ) const;

private:
	Vertex _to;
	Map<Vertex, DijkstraResult<Graph>> _results;
	Set<Vertex> _filter;
};

#include "Profiler.h"

CREATE_PROFILER_SET( shortestPaths );

ShortestPaths<ShortcutGraph> shortestPaths(
	const ShortcutGraph&  graph,
	ShortcutGraph::Vertex source,
	double                maxDist,
	double                minDist,
	double                maxEdge
);

bool witnessSearch(
	const ShortcutGraph&  graph,
	ShortcutGraph::Vertex source,
	ShortcutGraph::Vertex target,
	ShortcutGraph::Vertex avoid,
	double                maxDist
);

bool usefulEdge(
	const WeightedGraph& graph,
	WeightedGraph::Edge  e
);

ShortestPaths<WeightedGraph> dijkstraSearch(
	const WeightedGraph& graph,
	WeightedGraph::Vertex source,
	WeightedGraph::Vertex target
);

#include "Dijkstra.inl"

#endif // DIJKSTRA_H