#ifndef CIHIERARCHYBUILDER_H
#define CIHIERARCHYBUILDER_H

#include "ShortcutHierarchyBuilder.h"

class CIHierarchyBuilder : public ShortcutHierarchyBuilder
{
public:
	CIHierarchyBuilder( FilePath filepath, const WeightedGraph& graph );

protected:
	Ptr<ShortcutHierarchy> buildInternal() const final;
};

FACTORY_CREATE_JSON( CIHierarchyBuilder, PathSolverBuilder, const WeightedGraph& )

#endif // CIHIERARCHYBUILDER_H