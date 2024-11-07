template <class GraphClass, class VertexPredicate, class EdgePredicate>
FilteredGraph<GraphClass, VertexPredicate, EdgePredicate>::FilteredGraph( GraphClass& g, VertexPredicate vp, EdgePredicate ep )
	: BaseGraph<boost::filtered_graph<typename GraphClass::graph_type, EdgePredicate, VertexPredicate>, typename GraphClass::graph_type::vertex_property_type, typename GraphClass::graph_type::edge_property_type>( 0, g._graph, ep, vp )
{

}