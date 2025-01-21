template <class B, class... Ts>
Ptr<B> Factory<B, Ts...>::create( const Str& name, Ts&&... args )
{
	auto search = _map.find( name );
	if (search == _map.end()) { return nullptr; }
	return search->second->createInternal( std::forward<Ts>( args )... );
}

template <class B, class... Ts>
Factory<B, Ts...>::Factory( const Str& name )
{
	_map[name] = this;
}

template <class D, class B, class... Ts>
template <class F>
DerivedFactory<D, B, Ts...>::DerivedFactory( const Str& name, F fabricator )
	: Factory<B, Ts...>( name ),
	  _fabricator( std::move( fabricator ) )
{

}

template <class D, class B, class... Ts>
Ptr<B> DerivedFactory<D, B, Ts...>::createInternal( Ts&&... args ) const
{
	return _fabricator( std::forward<Ts>( args )... );
}