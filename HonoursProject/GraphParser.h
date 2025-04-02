#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include "Util.h"

#include "JSON.h"

#include "WeightedGraph.h"
#include "Factory.h"

class GraphParser
{
public:
	GraphParser( FilePath cachePath );
	virtual ~GraphParser() = default;

	Ptr<WeightedGraph> create() const;

protected:
	virtual Ptr<WeightedGraph> createInternal() const = 0;

private:
	const FilePath _cachePath;
};

FACTORY_CREATE_BASE_JSON( GraphParser )

JSON_CREATE_FACTORY( GraphParser )

#endif // GRAPHPARSER_H