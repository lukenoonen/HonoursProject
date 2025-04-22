#include "CICHBuilder.hpp"
#include "BUContractionQueue.hpp"
#include "Logger.hpp"
#include "Profiler.hpp"
#include "ShortcutHierarchy.hpp"

CICHBuilder::CICHBuilder(
	FilePath filepath,
	FilePath buildTimesFilepath,
	FilePath hierarchyFilepath
)
	: CachedPathSolverBuilder<ContractionHierarchy>(
		  std::move(filepath),
		  std::move(buildTimesFilepath)
	  ),
	  _hierarchyFilepath(std::move(hierarchyFilepath))
{
}

CREATE_GLOBAL_PROFILER(total, cich_builder);

namespace
{
	Vec<Vec<WeightedGraph::Vertex>> calculateQs(
		const FilePath&      filepath,
		const WeightedGraph& graph
	)
	{
		ShortcutHierarchy hierarchy;
		const bool        success = deserialize(filepath, hierarchy);
		if (!success) { return {}; }

		Vec<Vec<WeightedGraph::Vertex>> qs(
			hierarchy.height(), Vec<WeightedGraph::Vertex>{}
		);
		graph.vertexMap(
			[&hierarchy, &qs](const auto v)
			{
				qs[hierarchy.vertexLevel(v)].push_back(v);
				return false;
			}
		);

		return qs;
	}
}  // namespace

CICHBuilder::Build CICHBuilder::buildInternal(const WeightedGraph& graph) const
{
	g_logger.log("Constructing contraction hierarchy (from hierarchy)...\n");

	START_PROFILER(total, cich_builder);

	const Vec<Vec<WeightedGraph::Vertex>> qs = calculateQs(
		_hierarchyFilepath, graph
	);
	if (qs.empty())
	{
		g_logger.log(
			"Failed to load hierarchy from {}\n", _hierarchyFilepath.string()
		);
		return {};
	}

	Ptr<ContractionHierarchy> result = std::make_unique<ContractionHierarchy>();

	ContractionGraph contractionGraph(graph);
	for (const auto& q : qs)
	{
		BUContractionQueue<ContractionGraph> queue(contractionGraph, q);
		queue.contract();
	}

	contractionGraph.finalize();
	result->set(std::move(contractionGraph));

	STOP_PROFILER(total, cich_builder);

	LOG_PROFILERS(cich_builder);
	auto times = READ_PROFILERS(cich_builder);
	CLEAR_PROFILERS(cich_builder);

	return Pair{std::move(result), std::move(times)};
}

FACTORY_BEGIN_JSON("cich", CICHBuilder, PathSolverBuilder)

	JSON_ARG_FALLBACK(Str, filepath, "")
	JSON_ARG_FALLBACK(Str, buildtimes, "")
	JSON_ARG(Str, hierarchy)

	FACTORY_FABRICATE(
		std::move(filepath),
		std::move(buildtimes),
		std::move(hierarchy)
	)

FACTORY_END()