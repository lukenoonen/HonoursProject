#include "TestInst.h"
#include "GraphParser.h"
#include "Logger.h"
#include "Profiler.h"

namespace
{
	TestInst::TestCaseMap mapTestCases( const TestInst::TestCases& testCases )
	{
		TestInst::TestCaseMap result;
		for (auto& testCase : testCases)
		{
			result[testCase->name()] = testCase.get();
		}
		return result;
	}
}

TestInst::TestInst(
	Str                         name,
	Ptr<GraphParser>            graphParser,
	Ptr<PathSolverBuilder>      authorityBuilder,
	Vec<Ptr<PathSolverBuilder>> pathSolverBuilders,
	TestCases                   testCases,
	Logger                      results
)
	: _name( std::move( name ) ),
	  _graphParser( std::move( graphParser ) ),
	  _authorityBuilder( std::move( authorityBuilder ) ),
	  _pathSolverBuilders( std::move( pathSolverBuilders ) ),
	  _testCases( std::move( testCases ) ),
	  _testCaseMap( mapTestCases( _testCases ) ),
	  _results( std::move( results ) )
{

}

bool TestInst::run() const
{
	Timer<> instTimer;

	instTimer.start();

	_results.open();

	g_logger.log( "Running TestInst {}\n", _name );

	g_logger.log( "Parsing graph...\n" );
	Ptr<WeightedGraph> graph = _graphParser->create();

	g_logger.log( "Building authority path solver...\n" );
	Ptr<PathSolver> authority = _authorityBuilder->create( *graph );

	Vec<Vec<TestCase::RunArg>> allArgs;
	allArgs.reserve( _testCases.size() );
	for (const auto& testCase : _testCases)
	{
		allArgs.emplace_back( testCase->runAuthority( *graph, authority.get(), _results ) );
	}

	for (const Ptr<PathSolverBuilder>& pathSolverBuilder : _pathSolverBuilders)
	{
		size_t failures = 0;
		g_logger.log( "Building path solver...\n" );
		Ptr<PathSolver> pathSolver = pathSolverBuilder->create( *graph );
		for (size_t i = 0; i < _testCases.size(); i++)
		{
			const auto& testCase = _testCases[i];
			const auto& args     = allArgs[i];

			failures += testCase->run( args, pathSolver.get(), _results );
		}
		g_logger.log( "{} failures detected\n", failures );
		_results.log( "# {} failures detected\n", failures );
		pathSolver.reset();
	}

	instTimer.stop();

	g_logger.log( "Finished TestInst {} in {} seconds\n", _name, instTimer.duration() );

	authority.reset();
	graph.reset();

	_results.close();

	return true;
}

const Str& TestInst::name() const
{
	return _name;
}

JSON_BEGIN( TestInst )

	JSON_ARG( Str, name )
	JSON_ARG( Ptr<GraphParser>, graph )
	JSON_ARG( Ptr<PathSolverBuilder>, authority )
	JSON_ARG( Vec<Ptr<PathSolverBuilder>>, pathSolvers )
	JSON_ARG( TestInst::TestCases, testCases )
	JSON_ARG( Logger, results )

	JSON_FABRICATE(
		std::move( name ),
		std::move( graph ),
		std::move( authority ),
		std::move( pathSolvers ),
		std::move( testCases ),
		std::move( results )
	)

JSON_END()