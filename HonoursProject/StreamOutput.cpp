#include "StreamOutput.h"

#include <iostream>

void StreamOutput::write( Str str )
{
	std::cout << str;
}

FACTORY_BEGIN_JSON( "stream", StreamOutput, Output )
	
	FACTORY_FABRICATE()

FACTORY_END()