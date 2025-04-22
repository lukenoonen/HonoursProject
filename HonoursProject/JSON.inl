#include "JSON.hpp"

template <class T>
inline T json_arg(const JSON& json, const char* name)
{
	return json[name].get<T>();
}

template <class T>
inline T json_arg_fallback(const JSON& json, const char* name, T fallback)
{
	return json.value(name, fallback);
}