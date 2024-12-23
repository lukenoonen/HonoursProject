#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "ShortcutGraph.h"

template <class Graph>
class DijkstraResult
{
public:
	using VertexDescriptor = typename Graph::VertexDescriptor;
	using EdgeDescriptor   = typename Graph::EdgeDescriptor;

public:
	DijkstraResult() = default;
	DijkstraResult( VertexDescriptor source );
	DijkstraResult(
		VertexDescriptor vertex,
		VertexDescriptor prev,
		EdgeDescriptor   edge,
		double           distance
	);

	void update( VertexDescriptor prev, EdgeDescriptor edge, double distance );

	bool operator<( const DijkstraResult& other ) const;

	VertexDescriptor vertex() const;
	VertexDescriptor prev() const;
	EdgeDescriptor   edge() const;
	double           distance() const;

	bool isOrigin() const;

private:
	VertexDescriptor _vertex;
	VertexDescriptor _prev;
	EdgeDescriptor   _edge;
	double           _distance;
};

template <class Graph>
class ShortestPaths
{
public:
	using VertexDescriptor = typename Graph::VertexDescriptor;

public:
	ShortestPaths( VertexDescriptor to );

	void insert( VertexDescriptor v, const DijkstraResult<Graph>& result );

	double distance( VertexDescriptor from ) const;

	void filter( VertexDescriptor vertex );

	template <class P>
	bool vertexMap( P predicate ) const;

	template <class P>
	bool pathMap( VertexDescriptor from, P predicate ) const;

private:
	VertexDescriptor _to;
	std::unordered_map<VertexDescriptor, DijkstraResult<Graph>> _results;
	std::unordered_set<VertexDescriptor> _filter;
};

ShortestPaths<ShortcutGraph> shortestPaths(
	const ShortcutGraph&            graph,
	ShortcutGraph::VertexDescriptor source,
	double                          maxDist,
	double                          minDist,
	double                          maxEdge
);

bool witnessSearch(
	const ShortcutGraph&            graph,
	ShortcutGraph::VertexDescriptor source,
	ShortcutGraph::VertexDescriptor target,
	ShortcutGraph::VertexDescriptor avoid,
	double                          maxDist
);

bool usefulEdge(
	const WeightedGraph& graph,
	WeightedGraph::EdgeDescriptor e
);

#include "Dijkstra.inl"

#endif // DIJKSTRA_H