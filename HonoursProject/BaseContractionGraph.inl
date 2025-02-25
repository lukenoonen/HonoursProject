#include "Dijkstra.h"

template<class V>
inline size_t BaseContractionGraph<V>::currentImportance() const
{
	return this->_filter.size();
}

template<class V>
inline BaseContractionGraph<V>::Contraction BaseContractionGraph<V>::contract( BaseContractionGraph<V>::Vertex v )
{
	using Vertex = BaseContractionGraph<V>::Vertex;

	this->_filter.insert( v );
	Vec<Contraction::Shortcut> shortcuts;
	this->neighbourEdgeMap( v, [v, this, &shortcuts]( const auto e1, const auto e2 ) {
		const Vertex v1 = this->other( e1, v );
		const Vertex v2 = this->other( e2, v );
		const ShortcutEdge& edge1 = this->get( e1 );
		const ShortcutEdge& edge2 = this->get( e2 );
		const double throughWeight = edge1.weight() + edge2.weight();
		const bool hasWitnessPath = witnessSearch( *this, v1, v2, throughWeight );
		if (!hasWitnessPath)
		{
			shortcuts.emplace_back( v1, v2, ShortcutEdge{ edge1, edge2 } );
		}
		return false;
	} );

	this->_filter.remove( v );
	size_t edgeCount = 0;
	this->edgeMap( v, [&edgeCount]( const auto e ) {
		edgeCount++;
		return false;
	} );

	return Contraction{ v, edgeCount, shortcuts };
}

template<class V>
inline void BaseContractionGraph<V>::applyContraction( Contraction contraction )
{
	this->_filter.insert( contraction._contracted );

	for (auto& [v1, v2, edge] : contraction._shortcuts)
	{
		this->addEdge( v1, v2, std::move( edge ) );
	}
}

template<class V>
inline BaseContractionGraph<V>::Contraction::Contraction(
	Vertex        contracted,
	size_t        edgeCount,
	Vec<Shortcut> shortcuts
)
	: _contracted( contracted ),
	  _edgeDifference( (int)shortcuts.size() - (int)edgeCount ),
	  _shortcuts( std::move( shortcuts ) )
{

}

template<class V>
inline BaseContractionGraph<V>::Vertex BaseContractionGraph<V>::Contraction::contracted() const
{
	return _contracted;
}

template<class V>
inline int BaseContractionGraph<V>::Contraction::edgeDifference() const
{
	return _edgeDifference;
}