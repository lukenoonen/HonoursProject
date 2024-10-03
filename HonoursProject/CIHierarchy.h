#ifndef CIHIERARCHY_H
#define CIHIERARCHY_H

#include "WeightedGraph.h"
#include "ShortcutGraph.h"

class CIHierarchy
{
public:
	CIHierarchy( const WeightedGraph& g );

	double shortestDistance( WeightedGraph::vertex_descriptor from, WeightedGraph::vertex_descriptor to ) const;
	double shortestPath( WeightedGraph::vertex_descriptor from, WeightedGraph::vertex_descriptor to ) const;

private:
	const WeightedGraph& _g;
	std::vector<ShortcutGraph> _hierarchy; 
};

#endif // CIHIERARCHY_H