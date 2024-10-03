template <class V, class E>
Graph<V,E>::vertex_descriptor Graph<V,E>::addVertex( V data )
{
	return boost::add_vertex( data, _graph );
}

template <class V, class E>
Graph<V, E>::edge_descriptor Graph<V, E>::addEdge( vertex_descriptor source, vertex_descriptor target, E data )
{
	return boost::add_edge( source, target, data, _graph ).first;
}

template <class V, class E>
Graph<V, E>::vertex_descriptor Graph<V, E>::source( edge_descriptor edge ) const
{
	return boost::source( edge, _graph );
}

template <class V, class E>
Graph<V, E>::vertex_descriptor Graph<V, E>::target( edge_descriptor edge ) const
{
	return boost::target( edge, _graph );
}

template <class V, class E>
V& Graph<V, E>::operator[]( vertex_descriptor v )
{
	return _graph[v];
}

template <class V, class E>
const V& Graph<V, E>::operator[]( vertex_descriptor v ) const
{
	return _graph[v];
}

template <class V, class E>
E& Graph<V, E>::operator[]( edge_descriptor e )
{
	return _graph[e];
}

template <class V, class E>
const E& Graph<V, E>::operator[]( edge_descriptor e ) const
{
	return _graph[e];
}

template <class V, class E>
Graph<V, E>::vertices_size_type Graph<V, E>::numVertices() const
{
	return boost::num_vertices( _graph );
}

template <class V, class E>
Graph<V, E>::edges_size_type Graph<V, E>::numEdges() const
{
	return boost::num_edges( _graph );
}

template <class V, class E>
Graph<V, E>::degree_size_type Graph<V, E>::degree( vertex_descriptor v ) const
{
	return boost::out_degree( v, _graph );
}


template <class V, class E>
template <class P>
void Graph<V, E>::vertexMap( P predicate ) const
{
	auto [it, end] = boost::vertices( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class V, class E>
template <class P>
void Graph<V, E>::vertexMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::adjacent_vertices( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class V, class E>
template <class P>
void Graph<V, E>::edgeMap( P predicate ) const
{
	auto [it, end] = boost::edges( _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}

template <class V, class E>
template <class P>
void Graph<V, E>::edgeMap( vertex_descriptor v, P predicate ) const
{
	auto [it, end] = boost::out_edges( v, _graph );
	while (it != end)
	{
		if (predicate( *it )) { break; }
		it++;
	}
}