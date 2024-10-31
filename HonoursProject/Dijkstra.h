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
	using vertex_descriptor = typename Graph::vertex_descriptor;

	using VertexSet = std::unordered_set<vertex_descriptor>;

public:
	ShortestPaths( vertex_descriptor to );

	void insert( vertex_descriptor v, const DijkstraResult<Graph>& result );

	double distance( vertex_descriptor from ) const;

	template <class P>
	void vertexMap( P predicate ) const;

	template <class P>
	void pathMap( vertex_descriptor from, P predicate ) const;

private:
	vertex_descriptor _to;
	std::unordered_map<vertex_descriptor, DijkstraResult<Graph>> _results;
};

template <class Graph>
ShortestPaths<Graph> dijkstraShortestPaths( Graph graph, typename Graph::vertex_descriptor source, double maxDist, double maxEdge );

template <class Graph>
bool dijkstraWitnessSearch( Graph graph, typename Graph::vertex_descriptor source, typename Graph::vertex_descriptor target, typename Graph::vertex_descriptor avoid, double minDist );

#include "Dijkstra.inl"

#endif // DIJKSTRA_H