#ifndef BUILDTIMESANALYSER_H
#define BUILDTIMESANALYSER_H

#include "Util.hpp"

#include "Analyser.hpp"

class BuildTimesAnalyser : public Analyser
{
public:
	class Entry;

public:
	BuildTimesAnalyser(Vec<Entry> entries, FilePath output);

	void analyse() const final;

private:
	Vec<Entry> _entries;
	FilePath   _output;
};

FACTORY_CREATE_JSON(BuildTimesAnalyser, Analyser)

class BuildTimesAnalyser::Entry
{
public:
	class File;

public:
	Entry() = default;
	Entry(Str name, Vec<File> files);

	[[nodiscard]] const Str&       name() const;
	[[nodiscard]] const Vec<File>& files() const;

private:
	Str       _name;
	Vec<File> _files;
};

JSON_CREATE(BuildTimesAnalyser::Entry)

class BuildTimesAnalyser::Entry::File
{
public:
	File() = default;
	File(Str type, FilePath filepath);

	[[nodiscard]] const Str&      type() const;
	[[nodiscard]] const FilePath& filepath() const;

private:
	Str      _type;
	FilePath _filepath;
};

JSON_CREATE(BuildTimesAnalyser::Entry::File)

#endif  // BUILDTIMESANALYSER_H