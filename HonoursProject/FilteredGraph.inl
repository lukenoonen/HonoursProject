template <class GraphClass, class VertexPredicate, class EdgePredicate>
FilteredGraph<GraphClass, VertexPredicate, EdgePredicate>::FilteredGraph( GraphClass& g, VertexPredicate vp, EdgePredicate ep )
	: BaseGraph( g, vp, ep )
{

}