#include "GraphAnalyser.hpp"
#include "Serialize.hpp"
#include "WeightedGraph.hpp"

GraphAnalyser::GraphAnalyser(Vec<Entry> entries, FilePath output)
	: _entries(std::move(entries)),
	  _output(std::move(output))
{
}

void GraphAnalyser::analyse() const
{
	Vec<JSON> entryJSONs;
	entryJSONs.reserve(_entries.size());
	for (const Entry& entry : _entries)
	{
		WeightedGraph graph;
		deserialize(entry.filepath(), graph);
		entryJSONs.push_back({
			{    "name",        entry.name()},
			{"vertices", graph.numVertices()},
			{   "edges",    graph.numEdges()},
		});
	}

	const JSON dataJSON = {
		{"graphs", std::move(entryJSONs)}
	};

	std::ofstream out(_output);
	out << std::setw(4) << dataJSON;
}

FACTORY_BEGIN_JSON("graph", GraphAnalyser, Analyser)

JSON_ARG(Vec<GraphAnalyser::Entry>, entries)
JSON_ARG(Str, output)

FACTORY_FABRICATE(std::move(entries), std::move(output))

FACTORY_END()

GraphAnalyser::Entry::Entry(Str name, FilePath filepath)
	: _name(std::move(name)),
	  _filepath(std::move(filepath))
{
}

const Str& GraphAnalyser::Entry::name() const
{
	return _name;
}

const FilePath& GraphAnalyser::Entry::filepath() const
{
	return _filepath;
}

JSON_BEGIN(GraphAnalyser::Entry)

	JSON_ARG(Str, name)
	JSON_ARG(Str, filepath)

	JSON_FABRICATE(std::move(name), std::move(filepath))

JSON_END()