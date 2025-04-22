#include "Profiler.hpp"

template <class T>
Timer<T>::Timer()
	: _duration(0.0)
{
}

template <class T>
void Timer<T>::start()
{
	_startTime = std::chrono::high_resolution_clock::now();
}

template <class T>
void Timer<T>::stop()
{
	const Time finish = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double, T> diff = finish - _startTime;
	_duration += diff.count();
}

template <class T>
void Timer<T>::clear()
{
	_duration = 0.0;
}

template <class T>
double Timer<T>::duration() const
{
	return _duration;
}