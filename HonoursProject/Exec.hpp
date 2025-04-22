#ifndef EXEC_H
#define EXEC_H

#include "Util.hpp"

template <class... Ts>
void exec(const std::format_string<Ts...>& fmt, Ts&&... args)
{
	const Str cmd = std::format(fmt, std::forward<Ts>(args)...);
	const int rc  = system(cmd.c_str());
	if (rc != 0) { std::exit(rc); }
}

#endif  // EXEC_H