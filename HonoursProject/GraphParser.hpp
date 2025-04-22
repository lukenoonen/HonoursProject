#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include "Util.hpp"

#include "JSON.hpp"

#include "Factory.hpp"
#include "WeightedGraph.hpp"

class GraphParser
{
public:
	explicit GraphParser(FilePath cachePath);
	virtual ~GraphParser() = default;

	[[nodiscard]] Ptr<WeightedGraph> create() const;

protected:
	[[nodiscard]] virtual Ptr<WeightedGraph> createInternal() const = 0;

private:
	FilePath _cachePath;
};

FACTORY_CREATE_BASE_JSON(GraphParser)

JSON_CREATE_FACTORY(GraphParser)

#endif  // GRAPHPARSER_H