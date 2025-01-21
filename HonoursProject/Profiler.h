#ifndef PROFILER_H
#define PROFILER_H

#include "Util.h"

#include <chrono>

class Timer
{
private:
	using Clock = std::chrono::high_resolution_clock;
	using Time = std::chrono::high_resolution_clock::time_point;

public:
	Timer();

	void start();
	void stop();

	void clear();

	double duration() const;

private:
	double _duration;
	Time   _startTime;
};

class Profiler;

// ====================================
//             ProfilerSet  
// ====================================
class ProfilerSet
{
public:
	ProfilerSet( const char* name );

	void append( Profiler* profiler );

	void start( Profiler* profiler );
	void stop();

	void log() const;
	void clear();

	const char* name() const;

private:
	const char*      _name;
	Vec<Profiler*>   _profilers;
	Stack<Profiler*> _localStack;
};

// ====================================
//              Profiler
// ====================================
class Profiler : Timer
{
private:
	friend class ProfilerSet;

	using Clock = std::chrono::high_resolution_clock;
	using Time  = std::chrono::high_resolution_clock::time_point;

	static ProfilerSet _defaultSet;

public:
	Profiler( const char* name, bool global, ProfilerSet* set );

	void start();
	void stop();

private:
	void startInternal();
	void stopInternal();

private:
	const char*  _name;
	const bool   _global;
	double       _duration;
	Time         _startTime;
	ProfilerSet* _set;
};

// ====================================
//           ScopedProfiler  
// ====================================
class ScopedProfiler
{
public:
	ScopedProfiler( Profiler* base );
	~ScopedProfiler();

private:
	Profiler* _base;
};

#define CREATE_PROFILER_SET( set ) \
	inline ProfilerSet profiler_set_##set( #set )

#define CREATE_GLOBAL_PROFILER( name, set ) \
	static Profiler profiler_##set##_##name( #name, true, &profiler_set_##set )

#define CREATE_LOCAL_PROFILER( name, set ) \
	static Profiler profiler_##set##_##name( #name, false, &profiler_set_##set )

#define SCOPED_PROFILER( name, set ) \
	ScopedProfiler scoped_profiler_##name( &profiler_##set##_##name )

#define START_PROFILER( name, set ) \
	profiler_##set##_##name.start()

#define STOP_PROFILER( name, set ) \
	profiler_##set##_##name.stop()

#define LOG_PROFILERS( set ) \
	profiler_set_##set.log()

#define CLEAR_PROFILERS( set ) \
	profiler_set_##set.clear()

#endif // PROFILER_H