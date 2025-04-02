#ifndef EXEC_H
#define EXEC_H

#include "Util.h"

template <class... Ts>
void exec( const std::format_string<Ts...> fmt, Ts&&... args )
{
	const Str cmd = std::vformat( fmt.get(), std::make_format_args( args... ) );
	system( cmd.c_str() );
}

#endif // EXEC_H