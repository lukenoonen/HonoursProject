#ifndef OSMPARSER_H
#define OSMPARSER_H

#include "Util.hpp"

#include "GraphParser.hpp"

class OSMParser : public GraphParser
{
public:
	OSMParser(FilePath filepath, FilePath datapath);

protected:
	[[nodiscard]] Ptr<WeightedGraph> createInternal() const final;

private:
	FilePath _datapath;
};

FACTORY_CREATE_JSON(OSMParser, GraphParser)

#endif  // OSMPARSER_H