#ifndef FACTORY_H
#define FACTORY_H

#include "Util.hpp"

#include <functional>

// ====================================
//               Factory  
// ====================================
template <class B, class... Ts>
class Factory
{
private:
	using FactoryMap = Map<Str, Factory<B, Ts...>*>;

public:
	static Ptr<B> create( const Str& name, Ts&&... args );

public:
	explicit Factory( const Str& name );
	virtual ~Factory()                 = default;
	Factory(const Factory&)            = default;
	Factory& operator=(const Factory&) = default;
	Factory(Factory&&)                 = default;
	Factory& operator=(Factory&&)      = default;

private:
	virtual Ptr<B> createInternal( Ts&&... args ) const = 0;

private:
	inline static FactoryMap _map{};
};

#define FACTORY_CREATE_BASE( ... ) \
	template class Factory<__VA_ARGS__>;

#define FACTORY_CREATE_BASE_JSON( ... ) \
	FACTORY_CREATE_BASE( __VA_ARGS__, const JSON& )

// ====================================
//            DerivedFactory  
// ====================================
template <class D, class B, class... Ts>
class DerivedFactory : public Factory<B, Ts...>
{
public:
	template <class F>
	DerivedFactory( const Str& name, F fabricator );

private:
	Ptr<B> createInternal( Ts&&... args ) const final;

private:
	std::function<Ptr<B>( Ts&&... )> _fabricator;
};

#define FACTORY_CREATE( derived, base, ... ) \
	template class DerivedFactory<derived, base, __VA_ARGS__>;

#define FACTORY_BEGIN( name, derived, base )              \
	namespace detail_##derived { using Derived = derived; \
	static DerivedFactory<derived, base>                  \
		factory_##derived( name, []() {

#define FACTORY_FABRICATE( ... ) \
	return std::make_unique<Derived>( __VA_ARGS__ );

#define FACTORY_END() \
	} ); }

#define FACTORY_CREATE_JSON( derived, base ) \
	FACTORY_CREATE( derived, base, const JSON& )

#define FACTORY_BEGIN_JSON( name, derived, base )         \
	namespace detail_##derived { using Derived = derived; \
	static DerivedFactory<derived, base, const JSON&>     \
		factory_##derived( name, [](const JSON& json) {

#define FACTORY_CREATE_CACHE( derived, base ) \
	FACTORY_CREATE( derived, base, FilePath )

#define FACTORY_DEFINE_CACHE( name, derived, base ) \
	FACTORY_BEGIN( name, derived, base, FilePath )  \
	auto result = std::make_unique<derived>();      \
	deserialize( filepath, *result );               \
	return result;                                  \
	FACTORY_END()

#include "Factory.inl"

#endif // FACTORY_H