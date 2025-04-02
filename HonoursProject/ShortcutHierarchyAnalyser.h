#ifndef SHORTCUTHIERARCHYANALYSER_H
#define SHORTCUTHIERARCHYANALYSER_H

#include "Util.h"

#include "FileAnalyser.h"

class ShortcutHierarchyAnalyser : public FileAnalyser
{
public:
	ShortcutHierarchyAnalyser( FilePath filepath );

	void analyseFile( const FilePath& filepath ) const final;
};

FACTORY_CREATE_JSON( ShortcutHierarchyAnalyser, Analyser )

#endif // SHORTCUTHIERARCHYANALYSER_H