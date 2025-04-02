#include "Profiler.h"
#include "Logger.h"

#include <algorithm>

ProfilerSet::ProfilerSet( const char* name )
	: _name( name )
{
	g_logger.log( "Creating profiler set {}\n", name );
}

void ProfilerSet::append( Profiler* profiler )
{
	_profilers.push_back( profiler );
}

void ProfilerSet::start( Profiler* profiler )
{
	if (!_localStack.empty())
	{
		_localStack.top()->stopInternal();
	}

	_localStack.push( profiler );
}

void ProfilerSet::stop()
{
	_localStack.pop();

	if (!_localStack.empty())
	{
		_localStack.top()->startInternal();
	}
}

void ProfilerSet::log() const
{
	Vec<Profiler*> profilers = _profilers;

	std::sort( profilers.begin(), profilers.end(), []( const Profiler* lhs, const Profiler* rhs ) {
		return lhs->duration() < rhs->duration();
	} );

	g_logger.log( "\n===========================================\n" );
	g_logger.log( "| Performance for profiler set {}:\n", _name );
	for (const Profiler* profiler : profilers)
	{
		g_logger.log( "| {}: {:.4f} ms ({:.4f}%)\n", profiler->_name, profiler->duration(), 100.0 * profiler->duration() / profilers.back()->duration() );
	}
	g_logger.log( "===========================================\n\n" );
}

void ProfilerSet::clear()
{
	g_logger.log( "\n===========================================\n" );
	g_logger.log( "| Clearing profiler set {}...\n", _name );
	g_logger.log( "===========================================\n\n" );

	for (Profiler* profiler : _profilers)
	{
		profiler->clear();
	}
}

const char* ProfilerSet::name() const
{
	return _name;
}

ProfilerSet::Times ProfilerSet::times() const
{
	const Profiler* max = *std::max_element( _profilers.begin(), _profilers.end(), []( const Profiler* a, const Profiler* b ) {
		return a->duration() < b->duration();
	} );
	Times result;
	for (const Profiler* profiler : _profilers)
	{
		result[profiler->_name] = { profiler->duration(), 100.0 * profiler->duration() / max->duration() };
	}
	return result;
}

ProfilerSet Profiler::_defaultSet( "default" );

Profiler::Profiler( const char* name, bool global, ProfilerSet* set )
	: _name( name ),
	  _global( global ),
	  _duration( 0.0 ),
	  _set( set ? set : &_defaultSet )
{
	g_logger.log( "Creating {} profiler {} in set {}\n", global ? "global" : "local", name, _set->name() );
	_set->append( this );
}

void Profiler::start()
{
	if (!_global)
	{
		_set->start( this );
	}

	startInternal();
}

void Profiler::stop()
{
	stopInternal();

	if (!_global)
	{
		_set->stop();
	}
}

void Profiler::startInternal()
{
	Timer::start();
}

void Profiler::stopInternal()
{
	Timer::stop();
}

ScopedProfiler::ScopedProfiler( Profiler* base )
	: _base( base )
{
	_base->start();
}

ScopedProfiler::~ScopedProfiler()
{
	_base->stop();
}