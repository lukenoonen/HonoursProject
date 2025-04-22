#ifndef CACHEDPATHSOLVERBUILDER_H
#define CACHEDPATHSOLVERBUILDER_H

#include "Util.hpp"

#include "Profiler.hpp"

#include "PathSolverBuilder.hpp"

template <class T>
class CachedPathSolverBuilder : public PathSolverBuilder
{
protected:
	using Build = Opt<Pair<Ptr<T>, ProfilerSet::Times>>;

public:
	CachedPathSolverBuilder(FilePath filepath, FilePath buildTimesFilepath);
	~CachedPathSolverBuilder() override                                = default;
	CachedPathSolverBuilder(const CachedPathSolverBuilder&)            = default;
	CachedPathSolverBuilder& operator=(const CachedPathSolverBuilder&) = default;
	CachedPathSolverBuilder(CachedPathSolverBuilder&&)                 = default;
	CachedPathSolverBuilder& operator=(CachedPathSolverBuilder&&)      = default;

	void clearCache() const final;

protected:
	[[nodiscard]] Ptr<PathSolver> createInternal(
		const WeightedGraph& graph
	) const final;

private:
	[[nodiscard]] Ptr<PathSolver> load() const;
	[[nodiscard]] Ptr<PathSolver> build(const WeightedGraph& graph) const;

protected:
	virtual Build buildInternal(const WeightedGraph& graph) const = 0;

private:
	FilePath _filepath;
	FilePath _buildTimesFilepath;
};

#include "CachedPathSolverBuilder.inl"

#endif  // CACHEDPATHSOLVERBUILDER_H