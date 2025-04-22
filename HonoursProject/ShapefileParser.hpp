#ifndef SHAPEFILEPARSER_H
#define SHAPEFILEPARSER_H

#include "Util.hpp"

#include "GraphParser.hpp"

class ShapefileParser : public GraphParser
{
public:
	ShapefileParser(FilePath filepath, FilePath directory);

protected:
	[[nodiscard]] Ptr<WeightedGraph> createInternal() const final;

private:
	FilePath _directory;
};

FACTORY_CREATE_JSON(ShapefileParser, GraphParser)

#endif  // SHAPEFILEPARSER_H