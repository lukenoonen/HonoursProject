#ifndef OUTPUT_H
#define OUTPUT_H

#include "Util.h"

#include "JSON.h"
#include "Factory.h"

class Output
{
public:
	virtual ~Output() = default;
	virtual void write( const Str& str ) = 0;

	virtual void open() = 0;
	virtual void close() = 0;
};

FACTORY_CREATE_BASE_JSON( Output )

JSON_CREATE_FACTORY( Output )

#endif // OUTPUT_H