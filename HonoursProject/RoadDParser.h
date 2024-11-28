#ifndef ROADDPARSER_H
#define ROADDPARSER_H

#include "GraphParser.h"

class RoadDParser : public GraphParser
{
public:
	RoadDParser( std::filesystem::path filepath );

protected:
	std::optional<WeightedGraph> createInternal( std::ifstream file ) const final;
	std::ios_base::openmode getOpenMode() const final;
};

#endif // ROADDPARSER_H