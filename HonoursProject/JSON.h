#ifndef JSON_H
#define JSON_H

#include <json.hpp>

using JSON = nlohmann::json;

template <class T>
T json_arg( const JSON& json, const char* name );

template <class T>
T json_arg_fallback( const JSON& json, const char* name, T fallback );

#define JSON_CREATE( _type ) \
	void from_json( const JSON&, _type& );

#define JSON_BEGIN( _type )                                   \
	void from_json( const JSON& json, _type& data ) { \
		using Type = _type;

#define JSON_ARG( _type, _name ) \
	_type _name = json_arg<_type>( json, #_name );

#define JSON_ARG_FALLBACK( _type, _name, _fallback ) \
	_type _name = json_arg_fallback<_type>( json, #_name, _fallback );

#define JSON_ARG_SINGLE( _type, _name ) \
	_type _name = json_arg<_type>( json, nullptr );

#define JSON_ARG_SINGLE_FALLBACK( _type, _name, _fallback ) \
	_type _name = json_arg<_type>( json, nullptr, _fallback );

#define JSON_FABRICATE( ... ) \
	data = Type{ __VA_ARGS__ };

#define JSON_END() \
	}

#define JSON_CREATE_FACTORY( _type ) \
	JSON_CREATE( Ptr<_type> )

#define JSON_DEFINE_FACTORY( _type )                          \
	JSON_BEGIN( Ptr<_type> )                                  \
	JSON_ARG( Str, type )                                     \
	data = Factory<_type, const JSON&>::create( type, json ); \
	JSON_END()

#include "JSON.inl"

#endif // JSON_H