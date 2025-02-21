#ifndef CACHEDPATHSOLVERBUILDER_H
#define CACHEDPATHSOLVERBUILDER_H

#include "PathSolverBuilder.h"

template <class T>
class CachedPathSolverBuilder : public PathSolverBuilder
{
public:
	CachedPathSolverBuilder( FilePath filepath, const WeightedGraph& graph );

protected:
	Ptr<PathSolver> createInternal() const final;

private:
	Ptr<PathSolver> load() const;
	Ptr<PathSolver> build() const;

protected:
	virtual Ptr<T> buildInternal() const = 0;

private:
	FilePath _filepath;
};

#include "CachedPathSolverBuilder.inl"

#endif // CACHEDPATHSOLVERBUILDER_H