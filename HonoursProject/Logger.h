#ifndef LOGGER_H
#define LOGGER_H

#include "Util.h"

#include "Output.h"

class Logger
{
public:
	Logger();
	Logger( Ptr<Output> log, Ptr<Output> debug );
	Logger( Logger&& other ) = default;

	Logger& operator=( Logger&& other );

	template <class... Ts>
	void log( const std::format_string<Ts...> fmt, Ts&&... args );
	template <class... Ts>
	void debug( const std::format_string<Ts...> fmt, Ts&&... args );

private:
	Ptr<Output> _log;
	Ptr<Output> _debug;

	Queue<Str> _logQueue;
	Queue<Str> _debugQueue;
};



JSON_CREATE( Logger )

inline Logger g_logger{};

#include "Logger.inl"

#endif // LOGGER_H