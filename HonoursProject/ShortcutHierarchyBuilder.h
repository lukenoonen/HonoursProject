#ifndef SHORTCUTHIERARCHYBUILDER_H
#define SHORTCUTHIERARCHYBUILDER_H

#include "PathSolverBuilder.h"
#include "ShortcutHierarchy.h"

class ShortcutHierarchyBuilder : public PathSolverBuilder
{
public:
	ShortcutHierarchyBuilder( FilePath filepath, const WeightedGraph& graph );

protected:
	Ptr<PathSolver> createInternal() const final;

private:
	Ptr<PathSolver> load() const;
	Ptr<PathSolver> build() const;

protected:
	virtual Ptr<ShortcutHierarchy> buildInternal() const = 0;

private:
	FilePath _filepath;
};

#endif // SHORTCUTHIERARCHYBUILDER_H