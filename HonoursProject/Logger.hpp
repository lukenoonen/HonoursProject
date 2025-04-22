#ifndef LOGGER_H
#define LOGGER_H

#include "Util.hpp"

#include "Output.hpp"

class Logger
{
public:
	Logger() = default;
	explicit Logger(Ptr<Output> log);

	template <class... Ts>
	void log(const std::format_string<Ts...>& fmt, Ts&&... args) const;

	void open() const;
	void close() const;

protected:
	void logInternal(const Str& str) const;

	[[nodiscard]] bool ready() const;

private:
	Ptr<Output> _log;
};

JSON_CREATE(Logger)

class GlobalLogger : private Logger
{
public:
	GlobalLogger();
	explicit GlobalLogger(Ptr<Output> log);

	GlobalLogger& operator=(Logger&& other);

	template <class... Ts>
	void log(const std::format_string<Ts...>& fmt, Ts&&... args);

private:
	Queue<Str> _queue;
};

inline GlobalLogger g_logger{};

#include "Logger.inl"

#endif  // LOGGER_H