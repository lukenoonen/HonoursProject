#ifndef FILEOUTPUT_H
#define FILEOUTPUT_H

#include "Util.h"

#include "Output.h"

#include <fstream>

class FileOutput : public Output
{
public:
	FileOutput() = default;
	FileOutput( FilePath filepath );
	virtual ~FileOutput();

	void write( const Str& str ) final;

	void open() final;
	void close() final;

private:
	const FilePath _filepath;
	std::ofstream _file;
};

FACTORY_CREATE_JSON( FileOutput, Output )

#endif // FILEOUTPUT_H