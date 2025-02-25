#include "BUCHBuilder.h"
#include "BUContractionQueue.h"
#include "Profiler.h"
#include "Logger.h"

BUCHBuilder::BUCHBuilder( FilePath filepath )
	: CachedPathSolverBuilder<ContractionHierarchy>( std::move( filepath ) )
{

}

CREATE_GLOBAL_PROFILER( total, buch_builder );

Ptr<ContractionHierarchy> BUCHBuilder::buildInternal( const WeightedGraph& graph ) const
{
	g_logger.debug( "Constructing contraction hierarchy (bottom-up)...\n" );

	START_PROFILER( total, buch_builder );

	Ptr<ContractionHierarchy> result = std::make_unique<ContractionHierarchy>( graph );
	BUContractionQueue<ContractionGraph> queue( result->graph() );
	queue.contract();
	result->finalize();

	STOP_PROFILER( total, buch_builder );

	LOG_PROFILERS( buch_builder );
	CLEAR_PROFILERS( buch_builder );

	return result;
}

FACTORY_BEGIN_JSON( "buch", BUCHBuilder, PathSolverBuilder )

	JSON_ARG_FALLBACK( Str, filepath, "" )

	FACTORY_FABRICATE( std::move( filepath ) )

FACTORY_END()