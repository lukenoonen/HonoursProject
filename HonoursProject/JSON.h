#ifndef JSON_H
#define JSON_H

#include <json.hpp>

using JSON = nlohmann::json;

template <class T, class... Ts>
T json_arg( const JSON& json, Ts&&... args );

template <class T, class... Ts>
T json_arg_fallback( const JSON& json, T fallback, Ts&&... args );

template <class T>
T json_arg( const JSON& json );

template <class T>
T json_arg_fallback( const JSON& json, T fallback );

template <class T, class... Ts>
void from_json( const JSON& json, Vec<T>& data, Ts&&... args );

#define JSON_CREATE( _type, ... ) \
	void from_json( const JSON&, _type&, __VA_ARGS__ );

#define JSON_BEGIN( _type, ... )                                   \
	void from_json( const JSON& json, _type& data, __VA_ARGS__ ) { \
		using Type = _type;

#define JSON_ARG( _type, _name, ... ) \
	_type _name = json_arg<_type>( json[#_name], __VA_ARGS__ );

#define JSON_ARG_FALLBACK( _type, _name, _fallback, ... ) \
	_type _name = json_arg_fallback<_type>( json[#_name], _fallback, __VA_ARGS__ );

#define JSON_ARG_SINGLE( _type, _name, ... ) \
	_type _name = json_arg<_type>( json, __VA_ARGS__ );

#define JSON_ARG_SINGLE_FALLBACK( _type, _name, _fallback, ... ) \
	_type _name = json_arg<_type>( json, _fallback, __VA_ARGS__ );

#define JSON_FABRICATE( ... ) \
	data = Type{ __VA_ARGS__ };

#define JSON_END() \
	}

#define JSON_CREATE_FACTORY( _type, ... ) \
	JSON_CREATE( Ptr<_type>, __VA_ARGS__ )

#define JSON_DEFINE_FACTORY( _type )                          \
	JSON_BEGIN( Ptr<_type> )                                  \
	JSON_ARG( Str, type )                                     \
	data = Factory<_type, const JSON&>::create( type, json ); \
	JSON_END()

#define JSON_DEFINE_FACTORY_PARAM( _type, _types, _args, _params )                             \
	JSON_BEGIN( Ptr<_type>, VA_LIST _args )                                                    \
	JSON_ARG( Str, type )                                                                      \
	data = Factory<_type, const JSON&, VA_LIST _types>::create( type, json, VA_LIST _params ); \
	JSON_END()

#include "JSON.inl"

#endif // JSON_H