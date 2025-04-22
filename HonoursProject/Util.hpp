#ifndef UTIL_H
#define UTIL_H

#include <array>
#include <filesystem>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


#define VA_LIST(...) __VA_ARGS__

template <class T>
using Ptr = std::unique_ptr<T>;

template <class T>
using SharedPtr = std::shared_ptr<T>;

template <class T>
using Opt = std::optional<T>;

template <class T>
using Vec = std::vector<T>;

template <class T, class H = std::hash<T>, class C = std::equal_to<T>>
using Set = std::unordered_set<T, H, C>;

template <class T, class U, class H = std::hash<T>, class C = std::equal_to<T>>
using Map = std::unordered_map<T, U, H, C>;

using Str = std::string;

template <class T, class U>
using Pair = std::pair<T, U>;

using FilePath = std::filesystem::path;

template <class T>
using Stack = std::stack<T>;

template <class T>
using Queue = std::queue<T>;

template <class T, size_t N>
using Arr = std::array<T, N>;

#endif  // UTIL_H