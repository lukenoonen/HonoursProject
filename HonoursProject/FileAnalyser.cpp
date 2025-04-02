#include "FileAnalyser.h"

#include "Logger.h"

FileAnalyser::FileAnalyser( FilePath filepath )
	: _filepath( std::move( filepath ) )
{

}

void FileAnalyser::analyse() const
{
	g_logger.log( "Analysing file {}...\n", _filepath.string() );
	analyseFile( _filepath );
}

const FilePath& FileAnalyser::filepath() const
{
	return _filepath;
}