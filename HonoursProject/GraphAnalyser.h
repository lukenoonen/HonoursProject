#ifndef GRAPHANALYSER_H
#define GRAPHANALYSER_H

#include "Util.h"

#include "Analyser.h"

class GraphAnalyser : public Analyser
{
public:
	class Entry;

public:
	GraphAnalyser( Vec<Entry> entries, FilePath output );

	void analyse() const final;

private:
	Vec<Entry> _entries;
	FilePath   _output;
};

FACTORY_CREATE_JSON( GraphAnalyser, Analyser )

class GraphAnalyser::Entry
{
public:
	Entry() = default;
	Entry( Str name, FilePath filepath );
	
	const Str&      name() const;
	const FilePath& filepath() const;

private:
	Str      _name;
	FilePath _filepath;
};

JSON_CREATE( GraphAnalyser::Entry )

#endif // GRAPHANALYSER_H