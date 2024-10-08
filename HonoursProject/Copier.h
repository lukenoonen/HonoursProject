#ifndef COPIER_H
#define COPIER_H

namespace Copier
{
	struct None
	{
		template <class T1, class T2>
		void operator()( const T1&, T2& ) const
		{

		}
	};

	struct All
	{
		template <class T1, class T2>
		void operator()( const T1& src, T2& dest) const
		{
			dest = T2{ src };
		}
	};

	struct Pointer
	{
		template <class T1, class T2>
		void operator()( const T1& src, T2& dest ) const
		{
			dest = T2{ &src };
		}
	};
}

#endif // COPIER_H