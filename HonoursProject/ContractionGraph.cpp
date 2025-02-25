#include "ContractionGraph.h"

ContractionGraph::ContractionGraph( const WeightedGraph& source )
{
	source.vertexMap( [&source, this]( const auto v ) {
		addVertex( {} );
		return false;
	} );

	source.edgeMap( [&source, this]( const auto e ) {
		addEdge( source.source( e ), source.target( e ), { e, source[e].weight() } );
		return false;
	} );
}

void ContractionGraph::applyContraction( Contraction contraction )
{
	get( contraction.contracted() ).setImportance( currentImportance() );
	BaseContractionGraph<ContractionVertex>::applyContraction( std::move( contraction ) );
}

void ContractionGraph::finalize()
{
	_filter.clear();
}

void serialize( std::ostream& os, const ContractionGraph& data )
{
	using BaseType = BaseGraph<ContractionVertex, ShortcutEdge, VertexFilter>;
	serialize( os, (const BaseType&)(data) );
}

void deserialize( std::istream& is, ContractionGraph& data )
{
	using BaseType = BaseGraph<ContractionVertex, ShortcutEdge, VertexFilter>;
	deserialize( is, (BaseType&)(data) );
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