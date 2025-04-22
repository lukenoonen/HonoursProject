#include "ShortcutHierarchyAnalyser.hpp"

#include "ShortcutHierarchy.hpp"

#include "Logger.hpp"

ShortcutHierarchyAnalyser::ShortcutHierarchyAnalyser(FilePath filepath)
	: FileAnalyser(std::move(filepath))
{
}

void ShortcutHierarchyAnalyser::analyseFile(const FilePath& filepath) const
{
	ShortcutHierarchy hierarchy;
	if (!deserialize(filepath, hierarchy))
	{
		g_logger.log(
			"Failed to read shortcut hierarchy from {}\n", filepath.string()
		);
		return;
	}

	size_t level = 0;
	g_logger.log("Levels {}:\n", hierarchy._hierarchy.size());
	for (const auto& layer : hierarchy._hierarchy)
	{
		g_logger.log("Level {}:\n", level);
		g_logger.log(
			"|V|, |E| = {}, {}:\n", layer.numVertices(), layer.numEdges()
		);
		g_logger.log("V = \n[\n");
		layer.vertexMap(
			[&layer](const auto v)
			{
				g_logger.log("\t{}\n", layer.toSource(v) + 1);
				return false;
			}
		);
		g_logger.log("]\n");
		g_logger.log("E = \n[\n");
		layer.edgeMap(
			[&layer](const auto e)
			{
				const auto u = layer.source(e);
				const auto v = layer.target(e);
				g_logger.log(
					"\t({}, {}) : {}\n",
					layer.toSource(u) + 1,
					layer.toSource(v) + 1,
					layer[e].weight()
				);
				return false;
			}
		);
		g_logger.log("]\n");

		level++;
	}
}

FACTORY_BEGIN_JSON("shortcut_hierarchy", ShortcutHierarchyAnalyser, Analyser)

	JSON_ARG(Str, filepath)

	FACTORY_FABRICATE(std::move(filepath))

FACTORY_END()