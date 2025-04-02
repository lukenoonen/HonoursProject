#ifndef BUILDTIMESANALYSER_H
#define BUILDTIMESANALYSER_H

#include "Util.h"

#include "Analyser.h"

class BuildTimesAnalyser : public Analyser
{
public:
	class Entry;

public:
	BuildTimesAnalyser( Vec<Entry> entries, FilePath output );

	void analyse() const final;

private:
	Vec<Entry> _entries;
	FilePath   _output;
};

FACTORY_CREATE_JSON( BuildTimesAnalyser, Analyser )

class BuildTimesAnalyser::Entry
{
public:
	class File;

public:
	Entry() = default;
	Entry( Str name, Vec<File> files );
	
	const Str&       name() const;
	const Vec<File>& files() const;

private:
	Str       _name;
	Vec<File> _files;
};

JSON_CREATE( BuildTimesAnalyser::Entry )

class BuildTimesAnalyser::Entry::File
{
public:
	File() = default;
	File( Str type, FilePath filepath );

	const Str&      type() const;
	const FilePath& filepath() const;

private:
	Str      _type;
	FilePath _filepath;
};

JSON_CREATE( BuildTimesAnalyser::Entry::File )

#endif // BUILDTIMESANALYSER_H