#include "FileOutput.h"

FileOutput::FileOutput( FilePath filepath )
	: _filepath( std::move( filepath ) )
{

}

FileOutput::~FileOutput()
{

}

void FileOutput::write( const Str& str )
{
	_file << str;
}

void FileOutput::open()
{
	if (!_file.is_open())
	{
		_file.open( _filepath, std::ios::out );
	}
}

void FileOutput::close()
{
	if (_file.is_open())
	{
		_file.close();
	}
}

FACTORY_BEGIN_JSON( "file", FileOutput, Output )
	
	JSON_ARG( Str, filepath )
	
	FACTORY_FABRICATE( std::move( filepath ) )

FACTORY_END()