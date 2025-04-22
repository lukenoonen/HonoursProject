#ifndef SHORTCUTHIERARCHYANALYSER_H
#define SHORTCUTHIERARCHYANALYSER_H

#include "Util.hpp"

#include "FileAnalyser.hpp"

class ShortcutHierarchyAnalyser : public FileAnalyser
{
public:
	explicit ShortcutHierarchyAnalyser(FilePath filepath);

	void analyseFile(const FilePath& filepath) const final;
};

FACTORY_CREATE_JSON(ShortcutHierarchyAnalyser, Analyser)

#endif  // SHORTCUTHIERARCHYANALYSER_H