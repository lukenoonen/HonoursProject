#ifndef STREAMOUTPUT_H
#define STREAMOUTPUT_H

#include "Util.hpp"

#include "Output.hpp"

class StreamOutput : public Output
{
public:
	void write(const Str& str) final;

	void open() final;
	void close() final;
};

FACTORY_CREATE_JSON(StreamOutput, Output)

#endif  // STREAMOUTPUT_H