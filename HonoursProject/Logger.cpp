#include "Logger.h"
#include "StreamOutput.h"

Logger::Logger()
	: _log( std::make_unique<StreamOutput>() ),
	  _debug( std::make_unique<StreamOutput>() )
{

}

Logger::Logger( Ptr<Output> log, Ptr<Output> debug )
	: _log( std::move( log ) ),
	  _debug( std::move( debug ) )
{

}


Logger& Logger::operator=( Logger&& other )
{
	_log   = std::move( other._log );
	_debug = std::move( other._debug );

	while (!_logQueue.empty())
	{
		_log->write( _logQueue.front() );
		_logQueue.pop();
	}

	while (!_debugQueue.empty())
	{
		_debug->write( _debugQueue.front() );
		_debugQueue.pop();
	}

	return *this;
}

JSON_BEGIN( Logger )

	JSON_ARG( Ptr<Output>, log )
	JSON_ARG( Ptr<Output>, debug )

	JSON_FABRICATE(
		std::move( log ),
		std::move( debug )
	)

JSON_END()