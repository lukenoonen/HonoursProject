#include <fstream>

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
inline void serialize( std::ostream& os, const Vec<T>& data )
{
	serialize( os, data.size() );

	for (const T& item : data)
	{
		serialize( os, item );
	}
}

template <class T>
inline void deserialize( std::istream& is, Vec<T>& data )
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
inline void serialize( std::ostream& os, const Map<T, U>& data )
{
	serialize( os, data.size() );

	for (const auto& item : data)
	{
		serialize( os, item.first );
		serialize( os, item.second );
	}
}

template <class T, class U>
inline void deserialize( std::istream& is, Map<T, U>& data )
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
inline void serialize( std::ostream& os, const Pair<T, U>& data )
{
	serialize( os, data.first );
	serialize( os, data.second );
}

template<class T, class U>
inline void deserialize( std::istream& is, Pair<T, U>& data )
{
	deserialize( is, data.first );
	deserialize( is, data.second );
}

template<class T>
inline bool serialize( FilePath filepath, const T& data )
{
	std::ofstream file( filepath, std::ios::out | std::ios::binary );
	if (!file.is_open()) { return false; }
	try
	{
		serialize( file, data );
	}
	catch (...)
	{
		return false;
	}
	file.close();
	return true;
}

template<class T>
inline bool deserialize( FilePath filepath, T& data )
{
	std::ifstream file( filepath, std::ios::in | std::ios::binary );
	if (!file.is_open()) { return false; }
	try
	{
		deserialize( file, data );
	}
	catch (...)
	{
		return false;
	}
	file.close();
	return true;
}
