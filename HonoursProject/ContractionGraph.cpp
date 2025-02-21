#include "ContractionGraph.h"
#include "Dijkstra.h"

ContractionGraph::ContractionGraph( const WeightedGraph& source )
{
	source.vertexMap( [&source, this]( const auto v ) {
		addVertex( {}  );
		return false;
	} );

	source.edgeMap( [&source, this]( const auto e ) {
		addEdge( source.source( e ), source.target( e ), { e, source[e].weight() } );
		return false;
	} );
}

ContractionGraph::Contraction ContractionGraph::contract( ContractionGraph::Vertex v )
{
	Vec<Contraction::Shortcut> shortcuts;
	neighbourEdgeMap( v, [v, this, &shortcuts]( const auto e1, const auto e2 ) {
		const Vertex v1 = other( e1, v );
		const Vertex v2 = other( e2, v );
		const double throughWeight = get( e1 ).weight() + get( e2 ).weight();
		const bool hasWitnessPath = witnessSearch( *this, v1, v2, v, throughWeight );
		if (!hasWitnessPath)
		{
			shortcuts.emplace_back( v1, v2, ContractionEdge{ get( e1 ), get( e2 ) } );
		}
		return false;
	} );

	size_t edgeCount = 0;
	edgeMap( v, [&edgeCount]( const auto e ) {
		edgeCount++;
		return false;
	} );

	return Contraction{ v, shortcuts, edgeCount };
}

void ContractionGraph::applyContraction( Contraction contraction )
{
	contraction.apply( *this );
}

void ContractionGraph::finalize()
{
	_filter.filter.clear();
}

void serialize( std::ostream& os, const ContractionGraph& data )
{
	using BaseType = BaseGraph<ContractionVertex, ContractionEdge, VertexFilter>;
	serialize( os, (const BaseType&)(data) );
}

void deserialize( std::istream& is, ContractionGraph& data )
{
	using BaseType = BaseGraph<ContractionVertex, ContractionEdge, VertexFilter>;
	deserialize( is, (BaseType&)(data) );
}


ContractionGraph::Contraction::Contraction(
	Vertex        contracted,
	Vec<Shortcut> shortcuts,
	size_t        edgeCount
)
	: _contracted( contracted ),
	  _shortcuts( std::move( shortcuts ) ),
	  _edgeDifference( (int)_shortcuts.size() - (int)edgeCount )
{

}

int ContractionGraph::Contraction::edgeDifference() const
{
	return _edgeDifference;
}

void ContractionGraph::Contraction::apply( ContractionGraph& graph )
{
	graph[_contracted].setImportance( graph._filter.filter.size() );

	graph._filter.filter.insert( _contracted );

	for (auto& [v1, v2, edge] : _shortcuts)
	{
		graph.addEdge( v1, v2, std::move( edge ) );
	}
}

void ContractionVertex::setImportance( size_t importance )
{
	_importance = importance;
}

size_t ContractionVertex::importance() const
{
	return _importance;
}

void serialize( std::ostream& os, const ContractionVertex& data )
{
	serialize( os, data._importance );
}

void deserialize( std::istream& is, ContractionVertex& data )
{
	deserialize( is, data._importance );
}

ContractionEdge::ContractionEdge( WeightedGraph::Edge e, double weight )
	: _path( { e } ),
	_weight( weight )
{

}

ContractionEdge::ContractionEdge( const ContractionEdge& first, const ContractionEdge& second )
{
	const PathType& firstPath = first.path();
	const PathType& secondPath = second.path();

	// _path.reserve( firstPath.size() + secondPath.size() );

	// std::copy( firstPath.begin(), firstPath.end(), std::back_inserter( _path ) );
	// std::copy( secondPath.begin(), secondPath.end(), std::back_inserter( _path ) );

	_weight = first.weight() + second.weight();
}

const ContractionEdge::PathType& ContractionEdge::path() const
{
	return _path;
}

double ContractionEdge::weight() const
{
	return _weight;
}

void serialize( std::ostream& os, const ContractionEdge& data )
{
	serialize( os, data._path );
	serialize( os, data._weight );
}

void deserialize( std::istream& is, ContractionEdge& data )
{
	deserialize( is, data._path );
	deserialize( is, data._weight );
}