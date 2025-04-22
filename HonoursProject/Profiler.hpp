#ifndef PROFILER_H
#define PROFILER_H

#include "Util.hpp"

#include <chrono>

template <class T = std::ratio<1>>
class Timer
{
private:
	using Clock = std::chrono::high_resolution_clock;
	using Time  = std::chrono::high_resolution_clock::time_point;

public:
	Timer();
	virtual ~Timer()               = default;
	Timer(const Timer&)            = default;
	Timer& operator=(const Timer&) = default;
	Timer(Timer&&)                 = default;
	Timer& operator=(Timer&&)      = default;

	void start();
	void stop();

	void clear();

	[[nodiscard]] double duration() const;

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
	using Times = Map<Str, Pair<double, double>>;

public:
	explicit ProfilerSet(const char* name);

	void append(Profiler* profiler);

	void start(Profiler* profiler);
	void stop();

	void log() const;
	void clear();

	[[nodiscard]] const char* name() const;

	[[nodiscard]] Times times() const;

private:
	const char*      _name;
	Vec<Profiler*>   _profilers;
	Stack<Profiler*> _localStack;
};

// ====================================
//              Profiler
// ====================================
class Profiler : Timer<std::milli>
{
private:
	friend class ProfilerSet;

	using Clock = std::chrono::high_resolution_clock;
	using Time  = std::chrono::high_resolution_clock::time_point;

	static ProfilerSet _defaultSet;

public:
	Profiler(const char* name, bool global, ProfilerSet* set);

	void start();
	void stop();

private:
	void startInternal();
	void stopInternal();

private:
	const char*  _name;
	bool         _global;
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
	explicit ScopedProfiler(Profiler* base);
	~ScopedProfiler();

	ScopedProfiler(const ScopedProfiler&)            = delete;
	ScopedProfiler& operator=(const ScopedProfiler&) = delete;
	ScopedProfiler(ScopedProfiler&&)                 = delete;
	ScopedProfiler& operator=(ScopedProfiler&&)      = delete;

private:
	Profiler* _base;
};

#define CREATE_PROFILER_SET(set) inline ProfilerSet profiler_set_##set(#set)

#define CREATE_GLOBAL_PROFILER(name, set)                                     \
	static Profiler profiler_##set##_##name(#name, true, &profiler_set_##set)

#define CREATE_LOCAL_PROFILER(name, set)                                       \
	static Profiler profiler_##set##_##name(#name, false, &profiler_set_##set)

#define SCOPED_PROFILER(name, set)                                  \
	ScopedProfiler scoped_profiler_##name(&profiler_##set##_##name)

#define START_PROFILER(name, set) profiler_##set##_##name.start()

#define STOP_PROFILER(name, set) profiler_##set##_##name.stop()

#define READ_PROFILERS(set) profiler_set_##set.times()

#define LOG_PROFILERS(set) profiler_set_##set.log()

#define CLEAR_PROFILERS(set) profiler_set_##set.clear()

#include "Profiler.inl"

#endif  // PROFILER_H