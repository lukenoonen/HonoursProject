#ifndef CACHEDPATHSOLVERBUILDER_H
#define CACHEDPATHSOLVERBUILDER_H

#include "Util.h"

#include "Profiler.h"

#include "PathSolverBuilder.h"

template <class T>
class CachedPathSolverBuilder : public PathSolverBuilder
{
public:
	CachedPathSolverBuilder( FilePath filepath, FilePath buildTimesFilepath );
	virtual ~CachedPathSolverBuilder();

protected:
	Ptr<PathSolver> createInternal( const WeightedGraph& graph ) const final;

private:
	Ptr<PathSolver> load() const;
	Ptr<PathSolver> build( const WeightedGraph& graph ) const;

protected:
	using Build = Opt<Pair<Ptr<T>, ProfilerSet::Times>>;
	virtual Build buildInternal( const WeightedGraph& graph ) const = 0;

private:
	FilePath _filepath;
	FilePath _buildTimesFilepath;
};

#include "CachedPathSolverBuilder.inl"

#endif // CACHEDPATHSOLVERBUILDER_H