#ifndef CIHIERARCHYBUILDER_H
#define CIHIERARCHYBUILDER_H

#include "CachedPathSolverBuilder.hpp"
#include "ShortcutHierarchy.hpp"

#include "Profiler.hpp"

CREATE_PROFILER_SET(ci_builder);

class CIHierarchyBuilder : public CachedPathSolverBuilder<ShortcutHierarchy>
{
public:
	CIHierarchyBuilder(
		FilePath filepath,
		FilePath buildTimesFilepath,
		double   scale
	);

protected:
	[[nodiscard]] Build buildInternal(const WeightedGraph& graph) const final;

private:
	double _scale;
};

FACTORY_CREATE_JSON(CIHierarchyBuilder, PathSolverBuilder)

#endif  // CIHIERARCHYBUILDER_H