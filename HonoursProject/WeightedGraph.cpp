#include "WeightedGraph.h"
#include "Serialize.h"
#include "GraphParser.h"

#include <limits>

void WeightedGraph::normalize()
{
	double minWeight = std::numeric_limits<double>::infinity();
	edgeMap( [&minWeight, this]( const auto e ) {
		const double weight = get( e ).weight();
		if (minWeight > weight)
		{
			minWeight = weight;
		}
		return false;
	} );
	edgeMap( [&minWeight, this]( const auto e ) {
		get( e ).normalize( minWeight );
		return false;
	} );
}

void serialize( std::ostream& os, const WeightedGraph& data )
{
	using BaseType = BaseGraph<WeightedVertex, WeightedEdge, NoFilter>;
	serialize( os, (const BaseType&)(data) );
}

void deserialize( std::istream& is, WeightedGraph& data )
{
	using BaseType = BaseGraph<WeightedVertex, WeightedEdge, NoFilter>;
	deserialize( is, (BaseType&)(data) );
}

void serialize( std::ostream& os, const WeightedVertex& data )
{

}

void deserialize( std::istream& is, WeightedVertex& data )
{

}

WeightedEdge::WeightedEdge( double weight )
	: _weight( weight )
{

}

void WeightedEdge::normalize( double by )
{
	_weight /= by;
}

double WeightedEdge::weight() const
{
	return _weight;
}

void serialize( std::ostream& os, const WeightedEdge& data )
{
	serialize( os, data._weight );
}

void deserialize( std::istream& is, WeightedEdge& data )
{
	deserialize( is, data._weight );
}