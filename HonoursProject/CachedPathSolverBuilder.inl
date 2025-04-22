#include "CachedPathSolverBuilder.hpp"

#include "Logger.hpp"
#include "Profiler.hpp"

template <class T>
inline CachedPathSolverBuilder<T>::CachedPathSolverBuilder(
	FilePath filepath,
	FilePath buildTimesFilepath
)
	: _filepath(std::move(filepath)),
	  _buildTimesFilepath(std::move(buildTimesFilepath))
{
}

template <class T>
void inline CachedPathSolverBuilder<T>::clearCache() const
{
	std::filesystem::remove(_filepath);
	std::filesystem::remove(_buildTimesFilepath);
}

template <class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::createInternal(
	const WeightedGraph& graph
) const
{
	auto result = load();
	if (result) { return result; }
	g_logger.log("Failed to load path solver {}\n", _filepath.string());
	return build(graph);
}

template <class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::load() const
{
	g_logger.log("Loading path solver {}...\n", _filepath.string());
	Ptr<T> result = std::make_unique<T>();
	if (!_filepath.empty() && deserialize(_filepath, *result))
	{
		if (!_buildTimesFilepath.empty())
		{
			ProfilerSet::Times times;
			deserialize(_buildTimesFilepath, times);
			g_logger.log(
				"Path solver took {:.4f}s to build\n",
				times["total"].first * 0.001
			);
		}
		return result;
	}
	return nullptr;
}

template <class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::build(
	const WeightedGraph& graph
) const
{
	g_logger.log("Building path solver {}...\n", _filepath.string());
	Build result = buildInternal(graph);
	if (!result) { return nullptr; }

	auto& [solver, times] = *result;

	g_logger.log(
		"Path solver took {:.4f}s to build\n", times["total"].first * 0.001
	);

	if (!_filepath.empty()) { serialize(_filepath, *solver); }
	if (!_buildTimesFilepath.empty()) { serialize(_buildTimesFilepath, times); }
	return std::move(solver);
}