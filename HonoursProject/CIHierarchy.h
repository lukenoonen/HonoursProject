#ifndef CIHIERARCHY_H
#define CIHIERARCHY_H

#include "WeightedGraph.h"
#include "ShortcutGraph.h"
#include "Serialize.h"

class CIHierarchy
{
public:
	CIHierarchy() = default;
	CIHierarchy( const WeightedGraph& g );

	friend void serialize( std::ostream& os, const CIHierarchy& data );
	friend void deserialize( std::istream& is, CIHierarchy& data );

private:
	std::vector<ShortcutGraph> _hierarchy;
	std::vector<std::size_t>   _ladder;
};

void serialize( std::ostream& os, const CIHierarchy& data );
void deserialize( std::istream& is, CIHierarchy& data );

#endif // CIHIERARCHY_H