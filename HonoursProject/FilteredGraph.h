#ifndef FILTEREDGRAPH_H
#define FILTEREDGRAPH_H

#include "BaseGraph.h"

#include <boost/graph/filtered_graph.hpp>

template <class GraphClass, class VertexPredicate, class EdgePredicate>
class FilteredGraph : public BaseGraph<boost::filtered_graph<typename GraphClass::graph_type, VertexPredicate, EdgePredicate>>
{
public:
	FilteredGraph( GraphClass& g, VertexPredicate vp, EdgePredicate ep );
};

#include "FilteredGraph.inl"

#endif // FILTEREDGRAPH_H