#ifndef FILTER_H
#define FILTER_H

#include <boost/graph/filtered_graph.hpp>

#include <unordered_set>

namespace Filter
{
	using KeepAll = boost::keep_all;

	template <class T>
	struct Exclude
	{
		Exclude() = default;
		Exclude( const std::unordered_set<T>& exclude_ )
			: exclude( exclude_ )
		{

		}

		bool operator()( const T& t ) const
		{
			return !exclude.contains( t );
		}

		const std::unordered_set<T>& exclude;
	};
}

#endif // FILTER_H