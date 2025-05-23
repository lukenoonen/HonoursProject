#include "BaseContractionGraph.hpp"

#include "Dijkstra.hpp"

template <class V>
inline size_t BaseContractionGraph<V>::currentImportance() const
{
	return this->filter().size();
}

template <class V>
inline BaseContractionGraph<V>::Contraction BaseContractionGraph<
	V>::getContraction(typename BaseContractionGraph<V>::Vertex v)
{
	USING_GRAPH(BaseContractionGraph<V>);

	this->filter().insert(v);
	Vec<typename Contraction::Shortcut> shortcuts;
	this->neighbourEdgeMap(
		v,
		[v, this, &shortcuts](const auto e1, const auto e2) {
			const Vertex        v1            = this->other(e1, v);
			const Vertex        v2            = this->other(e2, v);
			const ShortcutEdge& edge1         = this->get(e1);
			const ShortcutEdge& edge2         = this->get(e2);
			const double        throughWeight = edge1.weight() + edge2.weight();
			const bool          hasWitnessPath = witnessSearch(
                *this, v1, v2, throughWeight
            );
			if (!hasWitnessPath)
			{
				shortcuts.emplace_back(v1, v2, ShortcutEdge{edge1, edge2});
			}
			return false;
		}
	);

	this->filter().remove(v);
	size_t edgeCount = 0;
	this->edgeMap(
		v,
		[&edgeCount](const auto) {
			edgeCount++;
			return false;
		}
	);

	return Contraction{v, edgeCount, shortcuts};
}

template <class V>
inline void BaseContractionGraph<V>::applyContraction(Contraction contraction)
{
	this->filter().insert(contraction._contractedVertex);

	for (auto& [v1, v2, edge] : contraction._shortcuts)
	{
		this->addEdge(v1, v2, std::move(edge));
	}
}

template <class V>
inline BaseContractionGraph<V>::Contraction::Contraction(
	Vertex        contractedVertex,
	size_t        edgeCount,
	Vec<Shortcut> shortcuts
)
	: _contractedVertex(contractedVertex),
	  _edgeDifference(static_cast<int>(shortcuts.size()) - static_cast<int>(edgeCount)),
	  _shortcuts(std::move(shortcuts))
{
}

template <class V>
inline BaseContractionGraph<V>::Vertex BaseContractionGraph<
	V>::Contraction::contractedVertex() const
{
	return _contractedVertex;
}

template <class V>
inline int BaseContractionGraph<V>::Contraction::edgeDifference() const
{
	return _edgeDifference;
}