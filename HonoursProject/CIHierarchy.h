#ifndef CIHIERARCHY_H
#define CIHIERARCHY_H

#include "WeightedGraph.h"
#include "ShortcutGraph.h"

class CIHierarchy
{
public:
	CIHierarchy( const WeightedGraph& g );

private:
	const WeightedGraph& _g;
	std::vector<ShortcutGraph> _hierarchy;
	std::vector<std::size_t> _ladder;
};

#endif // CIHIERARCHY_H