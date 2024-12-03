#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include "WeightedGraph.h"

#include <filesystem>
#include <fstream>

class GraphParser
{
protected:
	using VertexDescriptor = WeightedGraph::VertexDescriptor;
	using EdgeDescriptor = WeightedGraph::EdgeDescriptor;

public:
	GraphParser( std::filesystem::path filepath );

	std::optional<WeightedGraph> create() const;

protected:
	virtual std::optional<WeightedGraph> createInternal( std::ifstream file ) const = 0;
	virtual	std::ios_base::openmode getOpenMode() const = 0;

private:
	std::filesystem::path _filepath;
};

#endif // GRAPHPARSER_H