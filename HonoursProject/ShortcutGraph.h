#pragma once
#ifndef SHORTCUTGRAPH_H
#define SHORTCUTGRAPH_H

#include "BaseGraph.h"
#include "WeightedGraph.h"

class ShortcutVertex;
class ShortcutEdge;

class ShortcutGraph : public BaseGraph<ShortcutVertex, ShortcutEdge, VertexFilter>
{
public:
	ShortcutGraph() = default;
	ShortcutGraph( const WeightedGraph& source );
	ShortcutGraph( const ShortcutGraph& prev, const std::vector<VertexDescriptor>& discard );

	double length() const;

	ShortcutGraph::VertexDescriptor fromSource( WeightedGraph::VertexDescriptor v ) const;
	WeightedGraph::VertexDescriptor toSource( ShortcutGraph::VertexDescriptor v ) const;

private:
	void calculateMap();

public:
	friend void serialize( std::ostream& os, const ShortcutGraph& data );
	friend void deserialize( std::istream& is, ShortcutGraph& data );

private:
	std::unordered_map<VertexDescriptor, VertexDescriptor> _map;
};

void serialize( std::ostream& os, const ShortcutGraph& data );
void deserialize( std::istream& is, ShortcutGraph& data );

class ShortcutVertex
{
public:
	ShortcutVertex() = default;
	ShortcutVertex( WeightedGraph::VertexDescriptor mapped );

	WeightedGraph::VertexDescriptor mapped() const;

	friend void serialize( std::ostream& os, const ShortcutVertex& data );
	friend void deserialize( std::istream& is, ShortcutVertex& data );

private:
	WeightedGraph::VertexDescriptor _mapped;
};

void serialize( std::ostream& os, const ShortcutVertex& data );
void deserialize( std::istream& is, ShortcutVertex& data );

class ShortcutEdge
{
public:
	using PathType = WeightedGraph::EdgeVector;

public:
	ShortcutEdge() = default;
	ShortcutEdge( WeightedGraph::EdgeDescriptor e, double weight );
	ShortcutEdge( const ShortcutEdge& first, const ShortcutEdge& second );

	const PathType& path() const;
	double weight() const;
	double maxEdge() const;

	friend void serialize( std::ostream& os, const ShortcutEdge& data );
	friend void deserialize( std::istream& is, ShortcutEdge& data );

private:
	PathType _path;

	double _weight;
	double _maxEdge;
};

void serialize( std::ostream& os, const ShortcutEdge& data );
void deserialize( std::istream& is, ShortcutEdge& data );

#endif // SHORTCUTGRAPH_H