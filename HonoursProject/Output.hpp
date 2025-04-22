#ifndef OUTPUT_H
#define OUTPUT_H

#include "Util.hpp"

#include "Factory.hpp"
#include "JSON.hpp"

class Output
{
public:
	Output()                         = default;
	virtual ~Output()                = default;
	Output(const Output&)            = default;
	Output& operator=(const Output&) = default;
	Output(Output&&)                 = default;
	Output& operator=(Output&&)      = default;

	virtual void write(const Str& str) = 0;

	virtual void open()  = 0;
	virtual void close() = 0;
};

FACTORY_CREATE_BASE_JSON(Output)

JSON_CREATE_FACTORY(Output)

#endif  // OUTPUT_H