template <class VertexType, class EdgeType>
template <class VertexCopier, class EdgeCopier>
Graph<VertexType, EdgeType>::Graph( const Graph<VertexType, EdgeType>& other, VertexCopier vc, EdgeCopier ec )
	: BaseGraph( other, vc, ec )
{

}