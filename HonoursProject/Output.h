#ifndef OUTPUT_H
#define OUTPUT_H

#include "Util.h"

#include "JSON.h"
#include "Factory.h"

class Output
{
public:
	virtual void write( Str str ) = 0;
};

FACTORY_CREATE_BASE_JSON( Output )

JSON_CREATE_FACTORY( Output )

#endif // OUTPUT_H