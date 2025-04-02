#ifndef BUCHBUILDER_H
#define BUCHBUILDER_H

#include "Util.h"

#include "CachedPathSolverBuilder.h"
#include "ContractionHierarchy.h"

#include "Profiler.h"

CREATE_PROFILER_SET( buch_builder );

class BUCHBuilder : public CachedPathSolverBuilder<ContractionHierarchy>
{
public:
	BUCHBuilder( FilePath filepath, FilePath buildTimesFilepath );

protected:
	Build buildInternal( const WeightedGraph& graph ) const final;
};

FACTORY_CREATE_JSON( BUCHBuilder, PathSolverBuilder )

#endif // CIHIERARCHYBUILDER_H