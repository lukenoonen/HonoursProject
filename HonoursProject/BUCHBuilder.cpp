#include "BUCHBuilder.h"
#include "BUContractionQueue.h"
#include "Profiler.h"
#include "Logger.h"

BUCHBuilder::BUCHBuilder( FilePath filepath, FilePath buildTimesFilepath )
	: CachedPathSolverBuilder<ContractionHierarchy>( std::move( filepath ), std::move( buildTimesFilepath ) )
{

}

CREATE_GLOBAL_PROFILER( total, buch_builder );

BUCHBuilder::Build BUCHBuilder::buildInternal( const WeightedGraph& graph ) const
{
	g_logger.log( "Constructing contraction hierarchy (bottom-up)...\n" );

	START_PROFILER( total, buch_builder );

	Ptr<ContractionHierarchy> result = std::make_unique<ContractionHierarchy>();
	ContractionGraph contractionGraph( graph );
	BUContractionQueue<ContractionGraph> queue( contractionGraph );
	queue.contract();
	contractionGraph.finalize();
	result->set( std::move( contractionGraph ) );

	STOP_PROFILER( total, buch_builder );

	LOG_PROFILERS( buch_builder );
	auto times = READ_PROFILERS( buch_builder );
	CLEAR_PROFILERS( buch_builder );

	return Pair{ std::move( result ), std::move( times ) };
}

FACTORY_BEGIN_JSON( "buch", BUCHBuilder, PathSolverBuilder )

	JSON_ARG_FALLBACK( Str, filepath, "" )
	JSON_ARG_FALLBACK( Str, buildtimes, "" )

	FACTORY_FABRICATE( std::move( filepath ), std::move( buildtimes ) )

FACTORY_END()