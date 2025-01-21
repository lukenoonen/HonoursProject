#ifndef ROADDPARSER_H
#define ROADDPARSER_H

#include "Util.h"

#include "JSON.h"

#include "GraphParser.h"

class RoadDParser : public GraphParser
{
public:
	RoadDParser( FilePath filepath );

protected:
	Ptr<WeightedGraph> createInternal( std::ifstream file ) const final;
	std::ios_base::openmode getOpenMode() const final;
};

FACTORY_CREATE_JSON( RoadDParser, GraphParser )

#endif // ROADDPARSER_H