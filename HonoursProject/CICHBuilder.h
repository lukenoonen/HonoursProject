#ifndef CICHBUILDER_H
#define CICHBUILDER_H

#include "Util.h"

#include "CachedPathSolverBuilder.h"
#include "ContractionHierarchy.h"

#include "Profiler.h"

CREATE_PROFILER_SET( cich_builder );

class CICHBuilder : public CachedPathSolverBuilder<ContractionHierarchy>
{
public:
	CICHBuilder( FilePath filepath, FilePath hierarchyFilepath );

protected:
	Ptr<ContractionHierarchy> buildInternal( const WeightedGraph& graph ) const final;

private:
	FilePath _hierarchyFilepath;
};

FACTORY_CREATE_JSON( CICHBuilder, PathSolverBuilder )


#endif // CICHBUILDER_H