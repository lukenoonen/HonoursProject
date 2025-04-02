#ifndef OSMPARSER_H
#define OSMPARSER_H

#include "Util.h"

#include "JSON.h"

#include "GraphParser.h"

class OSMParser : public GraphParser
{
public:
	OSMParser( FilePath filepath, FilePath datapath );

protected:
	Ptr<WeightedGraph> createInternal() const final;

private:
	const FilePath _datapath;
};

FACTORY_CREATE_JSON( OSMParser, GraphParser )

#endif // OSMPARSER_H