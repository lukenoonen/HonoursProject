#ifndef BASEGRAPH_H
#define BASEGRAPH_H

#include <utility>
#include <vector>

template <class VertexType, class EdgeType>
class BaseGraph
{
public:
	using VertexDescriptor = size_t;
	using EdgeDescriptor   = std::pair<VertexDescriptor, size_t>;

private:
	using EdgeEntry   = std::pair<EdgeType, VertexDescriptor>;
	using EdgeList    = std::vector<EdgeEntry>;
	using VertexEntry = std::pair<VertexType, EdgeList>;
	using GraphType   = std::vector<VertexEntry>;

public:
	BaseGraph() = default;

	VertexDescriptor addVertex( VertexType data );
	EdgeDescriptor addEdge( VertexDescriptor source, VertexDescriptor target, EdgeType data );

	void removeVertices( const std::vector<VertexDescriptor>& remove );

	VertexDescriptor source( EdgeDescriptor e ) const;
	VertexDescriptor target( EdgeDescriptor e ) const;
	VertexDescriptor other( EdgeDescriptor e, VertexDescriptor v ) const;

	VertexType& get( VertexDescriptor v );
	const VertexType& get( VertexDescriptor v ) const;
	VertexType& operator[]( VertexDescriptor v );
	const VertexType& operator[]( VertexDescriptor v ) const;

	EdgeType& get( EdgeDescriptor e );
	const EdgeType& get( EdgeDescriptor e ) const;
	EdgeType& operator[]( EdgeDescriptor e );
	const EdgeType& operator[]( EdgeDescriptor e ) const;

	size_t numVertices() const;
	size_t numEdges() const;
	size_t degree( VertexDescriptor v ) const;

	template <class P>
	void vertexMap( P predicate ) const;
	template <class P>
	void vertexMap( VertexDescriptor v, P predicate ) const;
	template <class P>
	void edgeMap( P predicate ) const;
	template <class P>
	void edgeMap( VertexDescriptor v, P predicate ) const;

public:
	GraphType _graph;
	size_t    _numEdges;
};

#include "BaseGraph.inl"

#endif // BASEGRAPH_H