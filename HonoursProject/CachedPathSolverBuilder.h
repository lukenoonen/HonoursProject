#ifndef CACHEDPATHSOLVERBUILDER_H
#define CACHEDPATHSOLVERBUILDER_H

#include "Util.h"

#include "PathSolverBuilder.h"

template <class T>
class CachedPathSolverBuilder : public PathSolverBuilder
{
public:
	CachedPathSolverBuilder( FilePath filepath );

protected:
	Ptr<PathSolver> createInternal( const WeightedGraph& graph ) const final;

private:
	Ptr<PathSolver> load() const;
	Ptr<PathSolver> build( const WeightedGraph& graph ) const;

protected:
	virtual Ptr<T> buildInternal( const WeightedGraph& graph ) const = 0;

private:
	FilePath _filepath;
};

#include "CachedPathSolverBuilder.inl"

#endif // CACHEDPATHSOLVERBUILDER_H