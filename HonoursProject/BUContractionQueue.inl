#include "BUContractionQueue.h"
// From Geisberger et al. [2008]
#define EDGE_DIFFERENCE_WEIGHT			19
#define NEIGHBOURS_CONTRACTED_WEIGHT	12

template <class Graph>
inline BUContractionQueue<Graph>::BUContractionQueue( Graph& graph )
	: _graph( graph )
{
	graph.vertexMap( [&graph, this]( const auto v ) {
		initVertex( v );
		return false;
	} );
}

template<class Graph>
inline BUContractionQueue<Graph>::BUContractionQueue(
	Graph&             graph,
	const Vec<Vertex>& contractions
)
	: _graph( graph )
{
	for (const auto v : contractions)
	{
		initVertex( v );
	}
}

template<class Graph>
inline void BUContractionQueue<Graph>::contract()
{
	while (!empty())
	{
		_graph.applyContraction( pop() );
	}
}

template <class Graph>
inline BUContractionQueue<Graph>::Contraction BUContractionQueue<Graph>::pop()
{
	Contraction contraction;

	while (true)
	{
		const Heuristic& top = _queue.top();
		contraction = _graph.contract( top.vertex() );
		const int edgeDifference = contraction.edgeDifference();
		Heuristic edit = { top.vertex(), contraction.edgeDifference(), _contractedNeighbours };
		if (edit.heuristic() <= top.heuristic()) { break; }
		HeapHandle& handle = _handles[top.vertex()];
		*handle = std::move( edit );
		edit.update( edgeDifference );
		_queue.update( handle );
	}

	_graph.vertexMap( contraction.contracted(), [this]( const auto v ) {
		_contractedNeighbours[v]++;
		return false;
	} );

	_queue.pop();

	return contraction;
}

template <class Graph>
inline bool BUContractionQueue<Graph>::empty() const
{
	return _queue.empty();
}

template<class Graph>
inline void BUContractionQueue<Graph>::initVertex( Vertex v )
{
	const auto contraction = _graph.contract( v );
	_contractedNeighbours[v] = 0;
	_handles[v] = _queue.emplace( v, contraction.edgeDifference(), _contractedNeighbours );
}

template <class Graph>
inline BUContractionQueue<Graph>::Heuristic::Heuristic(
	Vertex                     vertex,
	int                        edgeDifference,
	const Map<Vertex, size_t>& contractedNeighbours
)
	: _vertex( vertex ),
      _edgeDifference( edgeDifference ),
	  _contractedNeighbours( &contractedNeighbours )
{

}

template <class Graph>
inline void BUContractionQueue<Graph>::Heuristic::update( int edgeDifference )
{
	_edgeDifference = edgeDifference;
}

template <class Graph>
inline bool BUContractionQueue<Graph>::Heuristic::operator<( const Heuristic& other ) const
{
	return heuristic() >= other.heuristic();
}

template <class Graph>
inline BUContractionQueue<Graph>::Heuristic::Vertex BUContractionQueue<Graph>::Heuristic::vertex() const
{
	return _vertex;
}

template <class Graph>
inline int BUContractionQueue<Graph>::Heuristic::edgeDifference() const
{
	return _edgeDifference;
}

template <class Graph>
inline int BUContractionQueue<Graph>::Heuristic::heuristic() const
{
	return EDGE_DIFFERENCE_WEIGHT       * _edgeDifference
		 + NEIGHBOURS_CONTRACTED_WEIGHT * (int)_contractedNeighbours->at( _vertex );
}