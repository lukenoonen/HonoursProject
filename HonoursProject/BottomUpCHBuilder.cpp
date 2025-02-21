#include "BottomUpCHBuilder.h"
#include "Dijkstra.h"
#include "Profiler.h"
#include "Logger.h"

#include <boost/heap/fibonacci_heap.hpp>

BottomUpCHBuilder::BottomUpCHBuilder( FilePath filepath, const WeightedGraph& graph )
	: CachedPathSolverBuilder<ContractionHierarchy>( std::move( filepath ), graph )
{

}

namespace
{
	class BottomUpHeuristic
	{
	public:
		BottomUpHeuristic( WeightedGraph::Vertex v, int edgeDifference );

		void update( int edgeDifference );

		bool operator<( const BottomUpHeuristic& other ) const;

		WeightedGraph::Vertex vertex() const;
		
		int edgeDifference() const;

	private:
		WeightedGraph::Vertex _v;
		int _edgeDifference;
	};

	BottomUpHeuristic::BottomUpHeuristic( WeightedGraph::Vertex v, int edgeDifference )
		: _v( v ),
		  _edgeDifference( edgeDifference )
	{

	}

	void BottomUpHeuristic::update( int edgeDifference )
	{
		_edgeDifference = edgeDifference;
	}

	bool BottomUpHeuristic::operator<( const BottomUpHeuristic& other ) const
	{
		return _edgeDifference > other._edgeDifference;
	}

	WeightedGraph::Vertex BottomUpHeuristic::vertex() const
	{
		return _v;
	}

	int BottomUpHeuristic::edgeDifference() const
	{
		return _edgeDifference;
	}
}

CREATE_GLOBAL_PROFILER( total, bottomup_ch_builder );
CREATE_GLOBAL_PROFILER( contract, bottomup_ch_builder );
CREATE_GLOBAL_PROFILER( apply_contraction, bottomup_ch_builder );

Ptr<ContractionHierarchy> BottomUpCHBuilder::buildInternal() const
{
	using Heap       = boost::heap::fibonacci_heap<BottomUpHeuristic>;
	using HeapHandle = Heap::handle_type;
	using HandleMap  = Map<WeightedGraph::Vertex, HeapHandle>;

	g_logger.debug( "Constructing contraction hierarchy (bottom-up)...\n" );

	START_PROFILER( total, bottomup_ch_builder );

	const WeightedGraph& source = graph();
	Ptr<ContractionHierarchy> result = std::make_unique<ContractionHierarchy>( source );

	HandleMap handles;
	Heap queue;
	source.vertexMap( [&result, &source, &queue, &handles]( const auto v ) {
		START_PROFILER( contract, bottomup_ch_builder );
		const auto contraction = result->contract( v );
		STOP_PROFILER( contract, bottomup_ch_builder );
		handles[v] = queue.emplace( v, contraction.edgeDifference() );
		return false;
	} );

	while (!queue.empty())
	{
		ContractionGraph::Contraction contraction;

		while (true)
		{
			const BottomUpHeuristic& top = queue.top();
			START_PROFILER( contract, bottomup_ch_builder );
			contraction = result->contract( top.vertex() );
			STOP_PROFILER( contract, bottomup_ch_builder );
			const int edgeDifference = contraction.edgeDifference();
			if (edgeDifference <= top.edgeDifference()) { break; }
			HeapHandle handle = handles.find( top.vertex() )->second;
			BottomUpHeuristic& edit = *handle;
			edit.update( edgeDifference );
			queue.increase( handle );
		}

		queue.pop();

		START_PROFILER( apply_contraction, bottomup_ch_builder );
		result->applyContraction( std::move( contraction ) );
		STOP_PROFILER( apply_contraction, bottomup_ch_builder );
	}

	result->finalize();

	STOP_PROFILER( total, bottomup_ch_builder );

	LOG_PROFILERS( bottomup_ch_builder );
	CLEAR_PROFILERS( bottomup_ch_builder );
	g_logger.debug( "\n" );

	return result;
}

FACTORY_BEGIN_JSON_PARAM( "bottomup_ch", BottomUpCHBuilder, PathSolverBuilder, (const WeightedGraph&), (const WeightedGraph& g) )

	JSON_ARG_FALLBACK( Str, filepath, "" )

	FACTORY_FABRICATE( std::move( filepath ), g )

FACTORY_END()