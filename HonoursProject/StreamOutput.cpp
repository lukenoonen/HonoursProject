#include "StreamOutput.h"

#include <iostream>

void StreamOutput::write( const Str& str )
{
	std::cout << str;
}

void StreamOutput::open()
{

}

void StreamOutput::close()
{

}

FACTORY_BEGIN_JSON( "stream", StreamOutput, Output )
	
	FACTORY_FABRICATE()

FACTORY_END()