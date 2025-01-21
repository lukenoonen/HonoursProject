template<class T, class ...Ts>
inline T json_arg( const JSON& json, Ts&& ...args )
{
	T result;
	from_json( json, result, std::forward<Ts>( args )... );
	return result;
}

template<class T, class ...Ts>
inline T json_arg_fallback( const JSON& json, T fallback, Ts&& ...args )
{
	try
	{
		T result;
		from_json( json, result, std::forward<Ts>( args )... );
		return result;
	}
	catch (const JSON::type_error&)
	{
		return fallback;
	}
}

template<class T>
inline T json_arg( const JSON& json )
{
	return json.get<T>();
}

template<class T>
inline T json_arg_fallback( const JSON& json, T fallback )
{
	try
	{
		return json.get<T>();
	}
	catch (const JSON::type_error&)
	{
		return fallback;
	}
}

template <class T, class... Ts>
inline void from_json( const JSON& json, Vec<T>& data, Ts&&... args )
{
	data.reserve( json.size() );
	for (auto it : json)
	{
		T item;
		from_json( it, item, std::forward<Ts>( args )... );
		data.emplace_back( std::move( item ) );
	}
}