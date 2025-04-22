#include "ContractionHierarchy.hpp"

#include <algorithm>
#include "Dijkstra.hpp"

namespace
{
	bool dijkstraStep(
		const ContractionGraph&                         graph,
		DijkstraData<ContractionGraph, DijkstraResult>& first,
		DijkstraData<ContractionGraph, DijkstraResult>& second,
		double&                                         bestDistance
	)
	{
		const auto& ex = first.extractMin();
		const auto  u  = ex.vertex();
		graph.edgeMap(
			u,
			[&ex, &graph, u, &first, &second, &bestDistance](const auto e) {
				const auto v = graph.other(e, u);
				if (first.closed(v)) { return false; }
				if (graph[v].importance() < graph[u].importance())
				{
					return false;
				}
				const double vDist   = first.distance(v);
				const double newDist = ex.distance() + graph[e].weight();
				if (newDist >= vDist) { return false; }

				first.decreasePriority(v, u, e, newDist);
				bestDistance = std::min(
					bestDistance, newDist + second.distance(v)
				);
				return false;
			}
		);

		return !first.empty() && ex.distance() < bestDistance;
	}
}  // namespace

void ContractionHierarchy::set(ContractionGraph graph)
{
	_contractionGraph = std::move(graph);
}

double ContractionHierarchy::distance(
	WeightedGraph::Vertex s,
	WeightedGraph::Vertex t
) const
{
	if (s == t) { return 0.0; }

	DijkstraData<ContractionGraph, DijkstraResult> forward(s);
	DijkstraData<ContractionGraph, DijkstraResult> backward(t);

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch  = true;
	bool backwardSearch = true;

	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = dijkstraStep(
				_contractionGraph, forward, backward, bestDistance
			);
		}
		if (backwardSearch)
		{
			backwardSearch = dijkstraStep(
				_contractionGraph, backward, forward, bestDistance
			);
		}
	}

	return bestDistance;
}

Vec<double> ContractionHierarchy::distances(Vertex s, const Vec<Vertex>& ts)
	const
{
	Vec<double> result;
	result.reserve(ts.size());
	for (const auto t : ts) { result.push_back(distance(s, t)); }
	return result;
}

void serialize(std::ostream& os, const ContractionHierarchy& data)
{
	serialize(os, data._contractionGraph);
}

void deserialize(std::istream& is, ContractionHierarchy& data)
{
	deserialize(is, data._contractionGraph);
}