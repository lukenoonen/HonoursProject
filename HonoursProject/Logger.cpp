#include "Logger.h"

Logger::Logger( Ptr<Output> log )
	: _log( std::move( log ) )
{

}

void Logger::open() const
{
	_log->open();
}

void Logger::close() const
{
	_log->close();
}

void Logger::logInternal( const Str& str ) const
{
	_log->write( str );
}

bool Logger::ready() const
{
	return _log != nullptr;
}

JSON_BEGIN( Logger )

	JSON_ARG( Ptr<Output>, log )

	JSON_FABRICATE( std::move( log ) )

JSON_END()

GlobalLogger::GlobalLogger()
	: Logger( nullptr )
{

}

GlobalLogger::GlobalLogger( Ptr<Output> log )
	: Logger( std::move( log ) )
{

}

GlobalLogger& GlobalLogger::operator=( Logger&& other )
{
	static_cast<Logger&>(*this) = std::move( other );

	while (!_queue.empty())
	{
		logInternal( _queue.front() );
		_queue.pop();
	}

	return *this;
}