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

	void write( Str str ) final;

private:
	std::ofstream _file;
};

FACTORY_CREATE_JSON( FileOutput, Output )

#endif // FILEOUTPUT_H