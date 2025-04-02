#ifndef CIHIERARCHYBUILDER_H
#define CIHIERARCHYBUILDER_H

#include "CachedPathSolverBuilder.h"
#include "ShortcutHierarchy.h"

#include "Profiler.h"

CREATE_PROFILER_SET( ci_builder );

class CIHierarchyBuilder : public CachedPathSolverBuilder<ShortcutHierarchy>
{
public:
	CIHierarchyBuilder( FilePath filepath, FilePath buildTimesFilepath, double scale );

protected:
	Build buildInternal( const WeightedGraph& graph ) const final;

private:
	const double _scale;
};

FACTORY_CREATE_JSON( CIHierarchyBuilder, PathSolverBuilder )

#endif // CIHIERARCHYBUILDER_H