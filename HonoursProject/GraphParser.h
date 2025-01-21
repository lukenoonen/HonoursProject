#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include "Util.h"

#include "JSON.h"

#include "WeightedGraph.h"
#include "Factory.h"

class GraphParser
{
protected:
	using Vertex = WeightedGraph::Vertex;
	using Edge   = WeightedGraph::Edge;

public:
	GraphParser( FilePath filepath );

	Ptr<WeightedGraph> create() const;

protected:
	virtual Ptr<WeightedGraph> createInternal( std::ifstream file ) const = 0;
	virtual	std::ios_base::openmode getOpenMode() const = 0;

private:
	FilePath _filepath;
};

FACTORY_CREATE_BASE_JSON( GraphParser )

JSON_CREATE_FACTORY( GraphParser )

#endif // GRAPHPARSER_H