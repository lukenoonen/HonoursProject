#ifndef FILEOUTPUT_H
#define FILEOUTPUT_H

#include "Util.hpp"

#include "Output.hpp"

#include <fstream>

class FileOutput : public Output
{
public:
	FileOutput() = default;
	explicit FileOutput( FilePath filepath );
	~FileOutput() override                   = default;
	FileOutput(const FileOutput&)            = delete;
	FileOutput& operator=(const FileOutput&) = delete;
	FileOutput(FileOutput&&)                 = delete;
	FileOutput& operator=(FileOutput&&)      = delete;

	void write( const Str& str ) final;

	void open() final;
	void close() final;

private:
	FilePath _filepath;
	std::ofstream _file;
};

FACTORY_CREATE_JSON( FileOutput, Output )

#endif // FILEOUTPUT_H