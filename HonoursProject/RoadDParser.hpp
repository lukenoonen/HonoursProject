#ifndef ROADDPARSER_H
#define ROADDPARSER_H

#include "Util.hpp"

#include "GraphParser.hpp"

class RoadDParser : public GraphParser
{
public:
	RoadDParser(FilePath filepath, FilePath datapath);

protected:
	[[nodiscard]] Ptr<WeightedGraph> createInternal() const final;

private:
	FilePath _datapath;
};

FACTORY_CREATE_JSON(RoadDParser, GraphParser)

#endif  // ROADDPARSER_H