#include "ShortcutHierarchy.hpp"

#include <algorithm>
#include "Dijkstra.hpp"

namespace
{
	bool dijkstraStep(
		const Vec<ShortcutGraph>&                    hierarchy,
		const Vec<size_t>&                           ladder,
		DijkstraData<ShortcutGraph, DijkstraResult>& first,
		DijkstraData<ShortcutGraph, DijkstraResult>& second,
		double&                                      bestDistance,
		const Vec<double>&                           maxDists
	)
	{
		const auto& ex    = first.extractMin();
		const auto  uS    = ex.vertex();
		const auto& graph = hierarchy[ladder[uS]];
		const auto  u     = graph.fromSource(uS);

		graph.edgeMap(
			u,
			[&ex,
			 &graph,
			 u,
			 uS,
			 &first,
			 &second,
			 &bestDistance,
			 &ladder,
			 &maxDists](const auto e) {
				const auto v  = graph.other(e, u);
				const auto vS = graph.toSource(v);

				if (first.closed(vS)) { return false; }

				const double vDist   = first.distance(vS);
				const double newDist = ex.distance() + graph[e].weight();

				if (newDist >= vDist) { return false; }
				if (newDist > maxDists[ladder[vS]]) { return false; }

				first.decreasePriority(vS, uS, e, newDist);
				bestDistance = std::min(
					bestDistance, newDist + second.distance(vS)
				);
				return false;
			}
		);

		return !first.empty() && ex.distance() < bestDistance;
	}
}  // namespace

ShortcutHierarchy::ShortcutHierarchy()
	: _scale(DEFAULT_SCALE)
{
}

ShortcutHierarchy::ShortcutHierarchy(const WeightedGraph& source, double scale)
	: _ladder(source.numVertices(), 0),
	  _scale(scale)
{
}

void ShortcutHierarchy::extend(
	ShortcutGraph                     layer,
	const Vec<ShortcutGraph::Vertex>& discard
)
{
	if (!_hierarchy.empty())
	{
		const size_t         idx  = _hierarchy.size() - 1;
		const ShortcutGraph& prev = top();
		for (const auto v : discard) { _ladder[prev.toSource(v)] = idx; }
	}

	_hierarchy.emplace_back(std::move(layer));
}

void ShortcutHierarchy::finalize()
{
	const size_t         idx     = _hierarchy.size() - 1;
	const ShortcutGraph& current = top();
	current.vertexMap(
		[&current, idx, this](const auto v) {
			_ladder[current.toSource(v)] = idx;
			return false;
		}
	);
}

const ShortcutGraph& ShortcutHierarchy::top() const
{
	return _hierarchy.back();
}

size_t ShortcutHierarchy::vertexLevel(Vertex v) const
{
	return _ladder[v];
}

size_t ShortcutHierarchy::height() const
{
	return _hierarchy.size();
}

namespace
{
	Vec<double> calculateMaxDists(double scale, size_t size)
	{
		Vec<double> result;
		result.reserve(size);
		result.emplace_back(scale);
		for (size_t i = 1; i < size; i++)
		{
			result.emplace_back(result.back() * scale);
		}
		return result;
	}
}  // namespace

double ShortcutHierarchy::distance(Vertex s, Vertex t) const
{
	if (s == t) { return 0.0; }

	const Vec<double> maxDists = calculateMaxDists(_scale, _hierarchy.size());

	DijkstraData<ShortcutGraph, DijkstraResult> forward(s);
	DijkstraData<ShortcutGraph, DijkstraResult> backward(t);

	double bestDistance = std::numeric_limits<double>::infinity();

	bool forwardSearch  = true;
	bool backwardSearch = true;

	while (forwardSearch || backwardSearch)
	{
		if (forwardSearch)
		{
			forwardSearch = dijkstraStep(
				_hierarchy, _ladder, forward, backward, bestDistance, maxDists
			);
		}
		if (backwardSearch)
		{
			backwardSearch = dijkstraStep(
				_hierarchy, _ladder, backward, forward, bestDistance, maxDists
			);
		}
	}

	return bestDistance;
}

Vec<double> ShortcutHierarchy::distances(Vertex s, const Vec<Vertex>& ts) const
{
	Vec<double> result;
	result.reserve(ts.size());
	for (const auto t : ts) { result.push_back(distance(s, t)); }
	return result;
}

void serialize(std::ostream& os, const ShortcutHierarchy& data)
{
	serialize(os, data._hierarchy);
	serialize(os, data._ladder);
	serialize(os, data._scale);
}

void deserialize(std::istream& is, ShortcutHierarchy& data)
{
	deserialize(is, data._hierarchy);
	deserialize(is, data._ladder);
	deserialize(is, data._scale);
}