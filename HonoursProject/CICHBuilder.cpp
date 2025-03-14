#include "CICHBuilder.h"
#include "BUContractionQueue.h"
#include "ShortcutHierarchy.h"
#include "Profiler.h"
#include "Logger.h"

CICHBuilder::CICHBuilder( FilePath filepath, FilePath hierarchyFilepath )
	: CachedPathSolverBuilder<ContractionHierarchy>( std::move( filepath ) ),
	  _hierarchyFilepath( std::move( hierarchyFilepath ) )
{

}

CREATE_GLOBAL_PROFILER( total, cich_builder );

Ptr<ContractionHierarchy> CICHBuilder::buildInternal( const WeightedGraph& graph ) const
{
	g_logger.debug( "Constructing contraction hierarchy (from hierarchy)...\n" );

	ShortcutHierarchy hierarchy;
	const bool success = deserialize( _hierarchyFilepath, hierarchy );
	if (!success)
	{
		g_logger.debug( "Failed to deserialize hierarchy {}\n", _hierarchyFilepath.string() );
		return nullptr;
	}

	START_PROFILER( total, cich_builder );

	Vec<Vec<WeightedGraph::Vertex>> qs( hierarchy.levels(), {} );
	graph.vertexMap( [&hierarchy, &qs]( const auto v ) {
		qs[hierarchy.level( v )].push_back( v );
		return false;
	} );

	Ptr<ContractionHierarchy> result = std::make_unique<ContractionHierarchy>( graph );
	for (const auto& q : qs)
	{
		BUContractionQueue<ContractionGraph> queue( result->graph(), q );
		queue.contract();
	}
	result->finalize();

	STOP_PROFILER( total, cich_builder );

	LOG_PROFILERS( cich_builder );
	CLEAR_PROFILERS( cich_builder );

	return result;
}

FACTORY_BEGIN_JSON( "cich", CICHBuilder, PathSolverBuilder )

	JSON_ARG_FALLBACK( Str, filepath, "" )
	JSON_ARG( Str, hierarchy )

	FACTORY_FABRICATE( std::move( filepath ), std::move( hierarchy ) )

FACTORY_END()