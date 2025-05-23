#include "CIHierarchyBuilder.hpp"
#include "BUContractionQueue.hpp"
#include "Dijkstra.hpp"
#include "Logger.hpp"
#include "Profiler.hpp"
#include "ShortcutHierarchy.hpp"

#include <algorithm>

namespace
{
	Vec<WeightedGraph::Edge> sortEdgesByWeight(const WeightedGraph& source)
	{
		Vec<WeightedGraph::Edge> edges;
		edges.reserve(source.numEdges());
		source.edgeMap(
			[&edges](const auto e) {
				edges.push_back(e);
				return false;
			}
		);

		std::ranges::sort(
			edges,
			[&source](const auto e1, const auto e2) {
				return source[e1].weight() < source[e2].weight();
			}
		);

		return edges;
	}

	size_t incrementEdgeIndexToWeight(
		double                          weight,
		const WeightedGraph&            source,
		const Vec<WeightedGraph::Edge>& edges,
		size_t                          index
	)
	{
		while (index < edges.size() && source[edges[index]].weight() <= weight)
		{
			index++;
		}

		return index;
	}

	Set<ShortcutGraph::Vertex> gatherEdgeVertices(
		size_t                          fromIndex,
		const Vec<WeightedGraph::Edge>& edges,
		const WeightedGraph&            g,
		const ShortcutGraph&            current
	)
	{
		Set<ShortcutGraph::Vertex> vertices;
		for (size_t i = fromIndex; i < edges.size(); i++)
		{
			vertices.insert(current.fromSource(g.source(edges[i])));
			vertices.insert(current.fromSource(g.target(edges[i])));
		}
		return vertices;
	}

	Set<ShortcutGraph::Vertex> selectVertices(
		double               maxWeight,
		double               minWeight,
		const ShortcutGraph& current
	)
	{
		Set<ShortcutGraph::Vertex> vertices;
		current.vertexMap(
			[&current, minWeight, maxWeight, &vertices](const auto origin) {
				if (vertices.contains(origin)) { return false; }

				const auto paths = shortestPaths(
					current, origin, maxWeight, minWeight
				);

				paths.vertexMap(
					[&paths, &vertices](const auto& result) {
						if (vertices.contains(result.vertex()))
						{
							return false;
						}

						const double midpointTarget = result.distance() * 0.5;

						double bestMidpointDistance = std::abs(
                            midpointTarget - result.distance()
                        );

						auto bestMidpoint = result.vertex();

						const bool addMidpoint = !paths.pathMap(
							result.vertex(),
							[&vertices,
							 midpointTarget,
							 &bestMidpointDistance,
							 &bestMidpoint](const auto result_) {
								if (vertices.contains(result_.vertex()))
								{
									return true;
								}

								const double midpointDistance = std::abs(
									midpointTarget - result_.distance()
								);

								if (midpointDistance < bestMidpointDistance)
								{
									bestMidpointDistance = midpointDistance;
									bestMidpoint         = result_.vertex();
								}

								return false;
							}
						);

						if (addMidpoint) { vertices.insert(bestMidpoint); }

						return false;
					}
				);

				return false;
			}
		);

		return vertices;
	}

	Vec<ShortcutGraph::Vertex> calculateDiscard(
		const Set<ShortcutGraph::Vertex>& keepSet,
		const ShortcutGraph&              current
	)
	{
		const size_t discardSize = current.numVertices() - keepSet.size();
		Vec<ShortcutGraph::Vertex> discard;
		discard.reserve(discardSize);

		current.vertexMap(
			[&keepSet, discardSize, &discard](const auto v) {
				if (!keepSet.contains(v)) { discard.push_back(v); }
				return discard.size() == discardSize;
			}
		);

		return discard;
	}
}  // namespace

CIHierarchyBuilder::CIHierarchyBuilder(
	FilePath filepath,
	FilePath buildTimesFilepath,
	double   scale
)
	: CachedPathSolverBuilder<ShortcutHierarchy>(
		  std::move(filepath),
		  std::move(buildTimesFilepath)
	  ),
	  _scale(scale)
{
}

CREATE_GLOBAL_PROFILER(total, ci_builder);
CREATE_GLOBAL_PROFILER(select_vertices, ci_builder);
CREATE_GLOBAL_PROFILER(extend, ci_builder);

CIHierarchyBuilder::Build CIHierarchyBuilder::buildInternal(
	const WeightedGraph& graph
) const
{
	g_logger.log("Constructing CI hierarchy...\n");

	START_PROFILER(total, ci_builder);

	const Vec<WeightedGraph::Edge> edges = sortEdgesByWeight(graph);

	double maxEdge   = 0.0;
	double maxWeight = 1.0;

	size_t maxEdgeIndex   = 0;
	size_t maxWeightIndex = incrementEdgeIndexToWeight(
		maxWeight, graph, edges, maxEdgeIndex
	);

	Ptr<ShortcutHierarchy> result = std::make_unique<ShortcutHierarchy>(
		graph, _scale
	);

	result->extend(
		{graph, edges, Pair{maxEdgeIndex, maxWeightIndex}},
		{}
	);

	while (true)
	{
		maxEdge = maxWeight;
		maxWeight *= _scale;

		maxEdgeIndex   = maxWeightIndex;
		maxWeightIndex = incrementEdgeIndexToWeight(
			maxWeight, graph, edges, maxEdgeIndex
		);

		const ShortcutGraph& top = result->top();

		Set<ShortcutGraph::Vertex> keepSet = gatherEdgeVertices(
			maxEdgeIndex, edges, graph, top
		);

		START_PROFILER(select_vertices, ci_builder);
		const Set<ShortcutGraph::Vertex> selectedSet = selectVertices(
			maxWeight, maxWeight * 0.75, top
		);
		STOP_PROFILER(select_vertices, ci_builder);

		for (const auto v : selectedSet) { keepSet.insert(v); }

		if (keepSet.empty()) { break; }

		Vec<ShortcutGraph::Vertex> const discard = calculateDiscard(keepSet, top);

		START_PROFILER(extend, ci_builder);
		
		ShortcutGraph layer(
			top, graph, edges, Pair{maxEdgeIndex, maxWeightIndex}
		);
		BUContractionQueue<ShortcutGraph> queue(layer, discard);
		queue.contract();
		layer.finalize();

		g_logger.log("Adding layer {} with (V, E) = ({}, {})\n", result->height(), layer.numVertices(), layer.numEdges());

		result->extend(std::move(layer), discard);
		
		STOP_PROFILER(extend, ci_builder);
	}

	result->finalize();

	STOP_PROFILER(total, ci_builder);

	LOG_PROFILERS(ci_builder);
	auto times = READ_PROFILERS(ci_builder);
	CLEAR_PROFILERS(ci_builder);

	return Pair{std::move(result), std::move(times)};
}

FACTORY_BEGIN_JSON("ci_hierarchy", CIHierarchyBuilder, PathSolverBuilder)

	JSON_ARG_FALLBACK(Str, filepath, "")
	JSON_ARG_FALLBACK(Str, buildtimes, "")
	JSON_ARG_FALLBACK(double, scale, ShortcutHierarchy::DEFAULT_SCALE)

	FACTORY_FABRICATE(std::move(filepath), std::move(buildtimes), scale)

FACTORY_END()