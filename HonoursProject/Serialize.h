#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "Util.h"

#include <ostream>
#include <istream>

template <class T>
void serialize( std::ostream& os, const T& data );
template <class T>
void deserialize( std::istream& is, T& data );

template <class T>
void serialize( std::ostream& os, const Vec<T>& data );
template <class T>
void deserialize( std::istream& is, Vec<T>& data );

template <class T, class U>
void serialize( std::ostream& os, const Map<T, U>& data );
template <class T, class U>
void deserialize( std::istream& is, Map<T, U>& data );

template <class T, class U>
void serialize( std::ostream& os, const Pair<T, U>& data );
template <class T, class U>
void deserialize( std::istream& is, Pair<T, U>& data );

void serialize( std::ostream& os, const Str& data );
void deserialize( std::istream& is, Str& data );

template <class T>
bool serialize( FilePath filepath, const T& data );
template <class T>
bool deserialize( FilePath filepath, T& data );

#include "Serialize.inl"

#endif // SERIALIZE_H