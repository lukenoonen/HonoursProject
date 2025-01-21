#include "TestCase.h"
#include "Profiler.h"
#include "Logger.h"

#include <random>

TestCase::TestCase( Str name )
	: _name( std::move( name ) )
{

}

size_t TestCase::run(
	const WeightedGraph&        graph,
	const PathSolver*           authority,
	const Vec<Ptr<PathSolver>>& pathSolvers
) const
{
	Timer totalTimer;
	size_t failures = 0;

	Vec<Endpoints> eps = endpoints( graph );

	g_logger.debug( "Running TestCase {}\n", _name );
	g_logger.log( "TestCase {}\n", _name );

	totalTimer.start();

	for (const auto& ep : eps)
	{
		const double authorityDistance = run( ep, authority );
		for (const auto& pathSolver : pathSolvers)
		{
			const double distance = run( ep, pathSolver.get(), authorityDistance );
			if (distance != authorityDistance) { failures++; }
		}
	}

	totalTimer.stop();

	g_logger.log( "{} {}\n", totalTimer.duration(), failures );
	g_logger.debug( "Finished TestCase {}\n", _name );

	return failures;
}

const Str& TestCase::name() const
{
	return _name;
}

double TestCase::run(
	const Endpoints&  endpoints,
	const PathSolver* pathSolver,
	double            authorityDistance /* = -1.0 */
) const
{
	Timer timer;
	timer.start();
	const double distance = pathSolver->distance( endpoints.u(), endpoints.v() );
	timer.stop();

	if (authorityDistance != -1.0)
	{
		const bool success = authorityDistance == distance;
		g_logger.log( "{} {}\n", timer.duration(), success );
	}
	else
	{
		g_logger.log( "{} {}\n", timer.duration(), distance );
	}
	return distance;
}

JSON_DEFINE_FACTORY( TestCase )

TestCase::Endpoints::Endpoints( size_t u, size_t v )
	: _u( u ),
	  _v( v )
{

}

size_t TestCase::Endpoints::u() const
{
	return _u;
}

size_t TestCase::Endpoints::v() const
{
	return _v;
}

JSON_BEGIN( TestCase::Endpoints )

	JSON_ARG( size_t, u )
	JSON_ARG( size_t, v )

	JSON_FABRICATE( u, v )

JSON_END()

TestCase::Result::Result( double distance, double duration )
	: _distance( distance ),
	  _duration( duration )
{

}

double TestCase::Result::distance() const
{
	return _distance;
}

double TestCase::Result::duration() const
{
	return _duration;
}

AllTestCase::AllTestCase( Str name )
	: TestCase( std::move( name ) )
{

}

Vec<TestCase::Endpoints> AllTestCase::endpoints( const WeightedGraph& ) const
{
	return {};
}

FACTORY_BEGIN_JSON( "all", AllTestCase, TestCase )

	JSON_ARG( Str, name )

	FACTORY_FABRICATE( std::move( name ) )

FACTORY_END()

ListTestCase::ListTestCase( Vec<Endpoints> endpoints, Str name )
	: TestCase( std::move( name ) ),
	  _endpoints( std::move( endpoints ) )
{

}

Vec<TestCase::Endpoints> ListTestCase::endpoints( const WeightedGraph& ) const
{
	return _endpoints;
}

FACTORY_BEGIN_JSON( "list", ListTestCase, TestCase )

	JSON_ARG( Vec<TestCase::Endpoints>, endpoints )
	JSON_ARG( Str,                      name )

	FACTORY_FABRICATE( std::move( endpoints ), std::move(name) )

FACTORY_END()

RandomTestCase::RandomTestCase( size_t trials, Str name )
	: TestCase( std::move( name ) ),
	  _trials( trials )
{

}

Vec<TestCase::Endpoints> RandomTestCase::endpoints( const WeightedGraph& graph ) const
{ 
	std::mt19937 rng( std::random_device{}() );
	std::uniform_int_distribution<size_t> dist( 0, graph.numVertices() );

	Vec<Endpoints> endpoints;
	endpoints.reserve( _trials );
	for (size_t i = 0; i < _trials; i++)
	{
		endpoints.emplace_back( dist( rng ), dist( rng ) );
	}

	return endpoints;
}

FACTORY_BEGIN_JSON( "random", RandomTestCase, TestCase )

	JSON_ARG( size_t, trials )
	JSON_ARG( Str,    name )

	FACTORY_FABRICATE( trials, std::move( name ) )

FACTORY_END()