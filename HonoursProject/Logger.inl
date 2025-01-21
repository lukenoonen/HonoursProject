#include <format>

template<class ...Ts>
inline void Logger::log( const std::format_string<Ts...> fmt, Ts&& ...args )
{
    Str message = std::vformat( fmt.get(), std::make_format_args( args... ) );
    if (_log)
    {
        _log->write( std::move( message ) );
    }
    else
    {
        _logQueue.emplace( std::move( message ) );
    }
}

template<class ...Ts>
inline void Logger::debug( const std::format_string<Ts...> fmt, Ts&& ...args )
{
    Str message = std::vformat( fmt.get(), std::make_format_args( args... ) );
    if (_log)
    {
        _debug->write( std::move( message ) );
    }
    else
    {
        _debugQueue.emplace( std::move( message ) );
    }
}