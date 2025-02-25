#ifndef BUCONTRACTIONQUEUE_H
#define BUCONTRACTIONQUEUE_H

#include "Util.h"

#include <boost/heap/fibonacci_heap.hpp>

template <class Graph>
class BUContractionQueue
{
public:
	using Contraction = typename Graph::Contraction;
	using Vertex      = typename Graph::Vertex;

private:
	class Heuristic;

	using Heap       = boost::heap::fibonacci_heap<Heuristic>;
	using HeapHandle = Heap::handle_type;

public:
	BUContractionQueue( Graph& graph );
	BUContractionQueue(
		Graph&             graph,
		const Vec<Vertex>& contractions
	);

	void contract();

private:
	Contraction pop();
	bool empty() const;

	void initVertex( Vertex v );

private:
	Graph&                  _graph;
	Map<Vertex, size_t>     _contractedNeighbours;
	Map<Vertex, HeapHandle> _handles;
	Heap                    _queue;
};

template <class Graph>
class BUContractionQueue<Graph>::Heuristic
{
private:
	using Vertex = typename Graph::Vertex;

public:
	Heuristic(
		Vertex                     vertex,
		int                        edgeDifference,
		const Map<Vertex, size_t>& contractedNeighbours
	);

	void update( int edgeDifference );

	bool operator<( const Heuristic& other ) const;

	Vertex vertex() const;
		
	int edgeDifference() const;
	int heuristic() const;

private:
	Vertex                     _vertex;
	int                        _edgeDifference;
	const Map<Vertex, size_t>* _contractedNeighbours;
};

#include "BUContractionQueue.inl"

#endif // BUCONTRACTIONQUEUE_H