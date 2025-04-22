#include "OSMParser.hpp"

#include "Exec.hpp"

#include "Logger.hpp"

OSMParser::OSMParser(FilePath filepath, FilePath datapath)
	: GraphParser(std::move(filepath)),
	  _datapath(std::move(datapath))
{
}

namespace
{
	template <class T>
	T convert(const Str& str)
	{
		std::stringstream stream(str);
		T                 result;
		stream >> result;
		return result;
	}
}  // namespace

Ptr<WeightedGraph> OSMParser::createInternal() const
{
	USING_GRAPH(WeightedGraph);

	g_logger.log("Loading graph data...\n");

	exec("osm4routing {}", _datapath.string());

	std::ifstream file("edges.csv", std::ios::in);

	Map<Vertex, Vertex> map;
	Ptr<WeightedGraph>  result = std::make_unique<WeightedGraph>();

	Str line;
	std::getline(file, line);  // Throw away keys
	while (std::getline(file, line))
	{
		std::stringstream stream(line);
		Str               temp;
		std::getline(stream, temp, ',');
		std::getline(stream, temp, ',');
		auto source = convert<Vertex>(temp);
		std::getline(stream, temp, ',');
		auto target = convert<Vertex>(temp);
		std::getline(stream, temp, ',');
		const auto weight = convert<double>(temp);

		if (auto search = map.find(source); search != map.end())
		{
			source = search->second;
		}
		else
		{
			const size_t v = result->numVertices();
			map[source]    = v;
			source         = v;
			result->addVertex({});
		}
		if (auto search = map.find(target); search != map.end())
		{
			target = search->second;
		}
		else
		{
			const size_t v = result->numVertices();
			map[target]    = v;
			target         = v;
			result->addVertex({});
		}

		if (stream.fail()) { return nullptr; }

		if (weight == 0.0)
		{
			map[target] = source;
		}
		else if (!result->edge(source, target))
		{
			result->addEdge(source, target, WeightedEdge{weight});
		}
	}

	file.close();

	g_logger.log("Cleaning up graph data...\n");

	std::filesystem::remove("nodes.csv");
	std::filesystem::remove("edges.csv");

	return result;
}

FACTORY_BEGIN_JSON("osm", OSMParser, GraphParser)

	JSON_ARG(Str, filepath)
	JSON_ARG(Str, datapath)

	FACTORY_FABRICATE(std::move(filepath), std::move(datapath))

FACTORY_END()