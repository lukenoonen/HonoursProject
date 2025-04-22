#ifndef FILEANALYSER_H
#define FILEANALYSER_H

#include "Util.hpp"

#include "Analyser.hpp"

class FileAnalyser : public Analyser
{
public:
	explicit FileAnalyser(FilePath filepath);
	~FileAnalyser() override                     = default;
	FileAnalyser(const FileAnalyser&)            = default;
	FileAnalyser& operator=(const FileAnalyser&) = default;
	FileAnalyser(FileAnalyser&&)                 = default;
	FileAnalyser& operator=(FileAnalyser&&)      = default;

protected:
	void         analyse() const final;
	virtual void analyseFile(const FilePath& filepath) const = 0;

	[[nodiscard]] const FilePath& filepath() const;

private:
	FilePath _filepath;
};

#endif  // FILEANALYSER_H