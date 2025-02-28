#ifndef SHAPEFILEPARSER_H
#define SHAPEFILEPARSER_H

#include "Util.h"

#include "JSON.h"

#include "GraphParser.h"

class ShapefileParser : public GraphParser
{
public:
	ShapefileParser( FilePath cachePath, FilePath filepath );

protected:
	Ptr<WeightedGraph> createInternal() const final;

private:
	FilePath _directory;
};

FACTORY_CREATE_JSON( ShapefileParser, GraphParser )

#endif // SHAPEFILEPARSER_H