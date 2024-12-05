#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "ShortcutGraph.h"

template <class Graph>
struct DijkstraResult
{
	using VertexDescriptor = typename Graph::VertexDescriptor;

	VertexDescriptor vertex;
	VertexDescriptor prev;
	double           distance;

	inline bool operator<( const DijkstraResult& other ) const
	{
		return distance < other.distance;
	}
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

	template <class P>
	void vertexMap( P predicate ) const;

	template <class P>
	void pathMap( VertexDescriptor from, P predicate ) const;

	size_t numVertices() const;

private:
	VertexDescriptor _to;
	std::unordered_map<VertexDescriptor, DijkstraResult<Graph>> _results;
};

ShortestPaths<ShortcutGraph> shortestPaths(
	const ShortcutGraph& graph,
	ShortcutGraph::VertexDescriptor source,
	double                          maxDist,
	double                          maxEdge
);

bool witnessSearch(
	const ShortcutGraph& graph,
	ShortcutGraph::VertexDescriptor source,
	ShortcutGraph::VertexDescriptor target,
	ShortcutGraph::VertexDescriptor avoid,
	double                          minDist
);

bool usefulEdge(
	const WeightedGraph& graph,
	WeightedGraph::EdgeDescriptor e
);

#include "Dijkstra.inl"

#endif // DIJKSTRA_H