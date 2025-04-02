#ifndef FILEANALYSER_H
#define FILEANALYSER_H

#include "Util.h"

#include "Analyser.h"

class FileAnalyser : public Analyser
{
public:
	FileAnalyser( FilePath filepath );
	virtual ~FileAnalyser() = default;

protected:
	void analyse() const final;
	virtual void analyseFile( const FilePath& filepath ) const = 0;

	const FilePath& filepath() const;

private:
	const FilePath _filepath;
};

#endif // FILEANALYSER_H