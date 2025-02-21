#ifndef BOTTOMUPCHBUILDER_H
#define BOTTOMUPCHBUILDER_H

#include "CachedPathSolverBuilder.h"
#include "ContractionHierarchy.h"

#include "Profiler.h"

CREATE_PROFILER_SET( bottomup_ch_builder );

class BottomUpCHBuilder : public CachedPathSolverBuilder<ContractionHierarchy>
{
public:
	BottomUpCHBuilder( FilePath filepath, const WeightedGraph& graph );

protected:
	Ptr<ContractionHierarchy> buildInternal() const final;
};

FACTORY_CREATE_JSON( BottomUpCHBuilder, PathSolverBuilder, const WeightedGraph& )

#endif // CIHIERARCHYBUILDER_H