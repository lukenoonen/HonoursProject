#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <unordered_map>
#include <unordered_set>

template <class Graph>
struct DijkstraResult
{
	Graph::vertex_descriptor vertex;
	Graph::vertex_descriptor prev;
	double distance;

	bool operator<( const DijkstraResult& other ) const
	{
		return distance < other.distance;
	}
};

template <class Graph>
class ShortestPaths
{
private:
	using VertexDescriptor = typename Graph::VertexDescriptor;

	using VertexSet = std::unordered_set<VertexDescriptor>;

public:
	ShortestPaths( VertexDescriptor to );

	void insert( VertexDescriptor v, const DijkstraResult<Graph>& result );

	double distance( VertexDescriptor from ) const;

	template <class P>
	void vertexMap( P predicate ) const;

	template <class P>
	void pathMap( VertexDescriptor from, P predicate ) const;

private:
	VertexDescriptor _to;
	std::unordered_map<VertexDescriptor, DijkstraResult<Graph>> _results;
};

template <class Graph>
ShortestPaths<Graph> dijkstraShortestPaths( Graph graph, typename Graph::VertexDescriptor source, double maxDist, double maxEdge );

template <class Graph>
bool dijkstraWitnessSearch( Graph graph, typename Graph::VertexDescriptor source, typename Graph::VertexDescriptor target, typename Graph::VertexDescriptor avoid, double minDist );

#include "Dijkstra.inl"

#endif // DIJKSTRA_H