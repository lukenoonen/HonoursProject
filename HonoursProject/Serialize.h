#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <ostream>
#include <istream>

#include <vector>
#include <unordered_map>

template <class T>
void serialize( std::ostream& os, const T& data );
template <class T>
void deserialize( std::istream& is, T& data );

template <class T>
void serialize( std::ostream& os, const std::vector<T>& data );
template <class T>
void deserialize( std::istream& is, std::vector<T>& data );

template <class T, class U>
void serialize( std::ostream& os, const std::unordered_map<T, U>& data );
template <class T, class U>
void deserialize( std::istream& is, std::unordered_map<T, U>& data );

template <class T, class U>
void serialize( std::ostream& os, const std::pair<T, U>& data );
template <class T, class U>
void deserialize( std::istream& is, std::pair<T, U>& data );

#include "Serialize.inl"

#endif // SERIALIZE_H