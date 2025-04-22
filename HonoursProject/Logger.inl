#include "Logger.hpp"

#include <format>

template <class... Ts>
inline void Logger::log(const std::format_string<Ts...>& fmt, Ts&&... args)
	const
{
	const Str message = std::format(fmt, std::forward<Ts>(args)...);
	logInternal(message);
}

template <class... Ts>
inline void GlobalLogger::log(const std::format_string<Ts...>& fmt, Ts&&... args)
{
	Str message = std::format(fmt, std::forward<Ts>(args)...);
	if (ready())
    {
        logInternal(message);
    }
	else
    {
        _queue.emplace(std::move(message));
    }
}