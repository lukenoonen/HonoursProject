#include "HDCalculator.hpp"
#include <algorithm>
#include "Dijkstra.hpp"

#include "Logger.hpp"

USING_GRAPH(WeightedGraph);

using Path = Pair<Vertex, Vertex>;

namespace
{
	class HDCalculatorWorker
	{
	public:
		explicit HDCalculatorWorker(const WeightedGraph& graph);

		[[nodiscard]] double distance(const Path& path) const;
		[[nodiscard]] double distance(Vertex v, const Path& path) const;

		size_t h(Vertex v, double r);

	private:
		[[nodiscard]] Vec<Set<Vertex>> getClosePathVertices(
			const Vec<Path>& paths
		) const;

		[[nodiscard]] Vec<Vertex> getPathsUnion(const Vec<Path>& paths) const;

		[[nodiscard]] Vec<Path> getClosePaths(Vertex v, double r, double d)
			const;

		[[nodiscard]] bool isSubpath(const Path& path_, const Path& path) const;

		[[nodiscard]] bool isRWitness(
			const Path& path_,
			const Path& path,
			double      r
		) const;

		[[nodiscard]] bool isRDClose(
			Vertex      v,
			const Path& path,
			double      r,
			double      d
		) const;

	private:
		const WeightedGraph*                              _graph;
		Vec<ShortestPaths<WeightedGraph, DijkstraResult>> _shortestPaths;
		Vec<Vec<Vertex>> _verticesAndNeighbours;
	};

	Vec<ShortestPaths<WeightedGraph, DijkstraResult>> allShortestPaths(
		const WeightedGraph& graph
	)
	{
		Vec<ShortestPaths<WeightedGraph, DijkstraResult>> shortestPaths;
		shortestPaths.reserve(graph.numVertices());
		graph.vertexMap(
			[&graph, &shortestPaths](const auto v)
			{
				shortestPaths.emplace_back(
					dijkstra<WeightedGraph, DijkstraResult>(graph, v)
				);
				return false;
			}
		);
		return shortestPaths;
	}

	Vec<Vec<Vertex>> allVerticesAndNeighbours(const WeightedGraph& graph)
	{
		Vec<Vec<Vertex>> result;
		result.reserve(graph.numVertices());
		graph.vertexMap(
			[&graph, &result](const auto v)
			{
				Vec<Vertex> vertexAndNeighbours;
				vertexAndNeighbours.reserve(1 + graph.degree(v));
				vertexAndNeighbours.push_back(v);
				graph.vertexMap(
					v,
					[&vertexAndNeighbours](const auto v_)
					{
						vertexAndNeighbours.push_back(v_);
						return false;
					}
				);
				result.emplace_back(std::move(vertexAndNeighbours));
				return false;
			}
		);
		return result;
	}

	HDCalculatorWorker::HDCalculatorWorker(const WeightedGraph& graph)
		: _graph(&graph),
		  _shortestPaths(allShortestPaths(graph)),
		  _verticesAndNeighbours(allVerticesAndNeighbours(graph))
	{
	}

	double HDCalculatorWorker::distance(const Path& path) const
	{
		return _shortestPaths[path.first].distance(path.second);
	}

	double HDCalculatorWorker::distance(Vertex v, const Path& path) const
	{
		double minDist = std::numeric_limits<double>::infinity();
		_shortestPaths[path.first].pathMap(
			path.second,
			[this, v, &minDist](const auto& result)
			{
				const auto dist = distance({v, result.vertex()});
				if (minDist > dist) { minDist = dist; }
				return false;
			}
		);
		return minDist;
	}

	template <class P>
	bool subsetApplyUtil(
		size_t             index,
		const Vec<Vertex>& set,
		Vec<Vertex>&       subset,
		P                  predicate
	)
	{
		if (index == set.size()) { return predicate(subset); }

		subset.push_back(set[index]);
		if (subsetApplyUtil(index + 1, set, subset, predicate)) { return true; }

		subset.pop_back();

		return subsetApplyUtil(index + 1, set, subset, predicate);
	}

	template <class P>
	void subsetApply(const Vec<Vertex>& set, P predicate)
	{
		Vec<Vertex> subset;
		subset.reserve(set.size());
		subsetApplyUtil(0, set, subset, predicate);
	}

	bool overlaps(const Set<Vertex>& vs, const Vec<Vertex>& set)
	{
		return std::ranges::any_of(
			set,
			[&vs](const auto v) {
				return vs.contains(v);
			}
		);
	}

	bool isHittingSet(const Vec<Set<Vertex>>& vertices, const Vec<Vertex>& set)
	{
		return std::ranges::all_of(
			vertices,
			[&set](const auto& vs) {
				return overlaps(vs, set);
			}
		);
	}

	size_t HDCalculatorWorker::h(Vertex v, double r)
	{
		const Vec<Path>        closePaths = getClosePaths(v, r, 2.0 * r);
		const Vec<Set<Vertex>> vertices   = getClosePathVertices(closePaths);
		const Vec<Vertex>      unionSet   = getPathsUnion(closePaths);

		size_t                 minH       = std::numeric_limits<size_t>::max();
		Vec<Vertex>            minSubset;
		subsetApply(
			unionSet,
			[&vertices, &minH, &minSubset](const auto& subset)
			{
				if (minH > subset.size() && isHittingSet(vertices, subset))
				{
					minSubset = subset;
					minH      = subset.size();
				}
				return false;
			}
		);

		g_logger.log("{}, {}\n", unionSet.size(), closePaths.size());

		for (const auto& path : closePaths)
		{
			g_logger.log("({}, {}) ", path.first, path.second);
		}

		g_logger.log("\n");

		for (const auto v_ : minSubset)
		{
			g_logger.log("{} ", v_);
		}

		g_logger.log("\n");
		return minH;
	}

	Vec<Set<Vertex>> HDCalculatorWorker::getClosePathVertices(
		const Vec<Path>& paths
	) const
	{
		Vec<Set<Vertex>> vertices;
		vertices.reserve(paths.size());
		for (const Path& path : paths)
		{
			Set<Vertex> set;
			_shortestPaths[path.first].pathMap(
				path.second,
				[&set](const auto& result) {
					set.insert(result.vertex());
					return false;
				}
			);
			vertices.emplace_back(std::move(set));
		}
		return vertices;
	}

	Vec<Vertex> HDCalculatorWorker::getPathsUnion(const Vec<Path>& paths) const
	{
		Set<Vertex> set;
		for (const Path& path : paths)
		{
			_shortestPaths[path.first].pathMap(
				path.second,
				[&set](const auto& result) {
					set.insert(result.vertex());
					return false;
				}
			);
		}
		return {set.begin(), set.end()};
	}

	Vec<Path> HDCalculatorWorker::getClosePaths(Vertex v, double r, double d)
		const
	{
		Vec<Path> closePaths;
		_graph->vertexMap(
			[this, r, d, v, &closePaths](const auto s)
			{
				_graph->vertexMap(
					[this, r, d, v, s, &closePaths](const auto t)
					{
						if (s >= t) { return false; }
						Path path = {s, t};
						if (isRDClose(v, path, r, d))
						{
							closePaths.emplace_back(std::move(path));
						}
						return false;
					}
				);
				return false;
			}
		);
		return closePaths;
	}

	bool HDCalculatorWorker::isSubpath(const Path& path_, const Path& path)
		const
	{
		return _shortestPaths[path.first].pathMap(
			path.second,
			[&path_](const auto& result)
			{
				const auto v = result.vertex();
				return v == path_.first || v == path_.second;
			}
		);
	}

	bool HDCalculatorWorker::isRWitness(
		const Path& path_,
		const Path& path,
		double      r
	) const
	{
		return distance(path_) >= r && isSubpath(path, path_);
	}

	bool HDCalculatorWorker::isRDClose(
		Vertex      v,
		const Path& path,
		double      r,
		double      d
	) const
	{
		const Vec<Vertex>& starts = _verticesAndNeighbours[path.first];
		const Vec<Vertex>& ends   = _verticesAndNeighbours[path.second];
		for (const auto s : starts)
		{
			for (const auto t : ends)
			{
				const Path path_ = {s, t};
				if (isRWitness(path_, path, r) && distance(v, path_) <= d)
				{
					return true;
				}
			}
		}
		return false;
	}
}  // namespace

HDCalculator::HDCalculator(Ptr<GraphParser> graphParser)
	: _graphParser(std::move(graphParser))
{
}

void HDCalculator::run() const
{
	Ptr<WeightedGraph> graph = _graphParser->create();
	HDCalculatorWorker worker(*graph);

	Vec<double> distances;
	distances.reserve(graph->numVertices() * graph->numVertices());
	graph->vertexMap(
		[&graph, &worker, &distances](const auto u)
		{
			graph->vertexMap(
				[&worker, u, &distances](const auto v)
				{
					const double dist = worker.distance({u, v});
					distances.push_back(dist * 0.5);
					distances.push_back(dist);
					return false;
				}
			);
			return false;
		}
	);
	std::ranges::sort(distances);

	size_t hd    = 0;
	double prevR = -1.0;
	for (const double r : distances)
	{
		if (prevR == r) { continue; }
		prevR = r;
		graph->vertexMap(
			[&worker, r, &hd](const auto v)
			{
				const size_t h = worker.h(v, r);
				hd = std::max(hd, h);
				g_logger.log("{}, {}: {}\n", v, r, h);
				return false;
			}
		);
		g_logger.log("\n");
	}
	g_logger.log("{}\n", hd);
}

JSON_BEGIN(HDCalculator)

	JSON_ARG(Ptr<GraphParser>, graph)

	JSON_FABRICATE(std::move(graph))

JSON_END()