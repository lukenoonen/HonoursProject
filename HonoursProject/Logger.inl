#include <format>

template<class ...Ts>
inline void Logger::log( const std::format_string<Ts...> fmt, Ts&& ...args ) const
{
    Str message = std::vformat( fmt.get(), std::make_format_args( args... ) );
    logInternal( message );
}

template<class ...Ts>
inline void GlobalLogger::log( const std::format_string<Ts...> fmt, Ts&& ...args )
{
    Str message = std::vformat( fmt.get(), std::make_format_args( args... ) );
    if (ready())
    {
        logInternal( message );
    }
    else
    {
        _queue.emplace( std::move( message ) );
    }
}