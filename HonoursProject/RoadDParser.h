#ifndef ROADDPARSER_H
#define ROADDPARSER_H

#include "Util.h"

#include "JSON.h"

#include "GraphParser.h"

class RoadDParser : public GraphParser
{
public:
	RoadDParser( FilePath filepath, FilePath datapath );

protected:
	Ptr<WeightedGraph> createInternal() const final;

private:
	const FilePath _datapath;
};

FACTORY_CREATE_JSON( RoadDParser, GraphParser )

#endif // ROADDPARSER_H