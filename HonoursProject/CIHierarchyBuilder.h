#ifndef CIHIERARCHYBUILDER_H
#define CIHIERARCHYBUILDER_H

#include "CachedPathSolverBuilder.h"
#include "ShortcutHierarchy.h"

#include "Profiler.h"

CREATE_PROFILER_SET( ci_builder );

class CIHierarchyBuilder : public CachedPathSolverBuilder<ShortcutHierarchy>
{
public:
	CIHierarchyBuilder( FilePath filepath, double scale );

protected:
	Ptr<ShortcutHierarchy> buildInternal( const WeightedGraph& graph ) const final;

private:
	const double _scale;
};

FACTORY_CREATE_JSON( CIHierarchyBuilder, PathSolverBuilder )

#endif // CIHIERARCHYBUILDER_H