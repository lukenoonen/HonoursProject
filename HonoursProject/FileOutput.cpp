#include "FileOutput.h"

FileOutput::FileOutput( FilePath filepath )
	: _file( filepath, std::ios::out )
{

}

FileOutput::~FileOutput()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

void FileOutput::write( Str str )
{
	_file << str;
}

FACTORY_BEGIN_JSON( "file", FileOutput, Output )
	
	JSON_ARG( Str, filepath )
	
	FACTORY_FABRICATE( std::move( filepath ) )

FACTORY_END()