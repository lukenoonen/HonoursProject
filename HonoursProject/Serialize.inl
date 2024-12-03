#include <ostream>
#include <istream>

#include <vector>
#include <unordered_map>

template<class T>
inline void serialize( std::ostream& os, const T& data )
{
	os.write( reinterpret_cast<const char*>(&data), sizeof( T ) );
}

template<class T>
inline void deserialize( std::istream& is, T& data )
{
	is.read( reinterpret_cast<char*>(&data), sizeof( T ) );
}

template <class T>
inline void serialize( std::ostream& os, const std::vector<T>& data )
{
	serialize( os, data.size() );

	for (const T& item : data)
	{
		serialize( os, item );
	}
}

template <class T>
inline void deserialize( std::istream& is, std::vector<T>& data )
{
	size_t size;
	deserialize( is, size );
	data.resize( size );

	for (T& item : data)
	{
		deserialize( is, item );
	}
}

template <class T, class U>
inline void serialize( std::ostream& os, const std::unordered_map<T, U>& data )
{
	serialize( os, data.size() );

	for (const auto& item : data)
	{
		serialize( os, item.first );
		serialize( os, item.second );
	}
}

template <class T, class U>
inline void deserialize( std::istream& is, std::unordered_map<T, U>& data )
{
	size_t size;
	deserialize( is, size );

	for (size_t i = 0; i < size; i++)
	{
		T key;
		deserialize( is, key );

		U value;
		deserialize( is, value );

		data[std::move( key )] = std::move( value );
	}
}

template<class T, class U>
inline void serialize( std::ostream& os, const std::pair<T, U>& data )
{
	serialize( os, data.first );
	serialize( os, data.second );
}

template<class T, class U>
inline void deserialize( std::istream& is, std::pair<T, U>& data )
{
	deserialize( is, data.first );
	deserialize( is, data.second );
}