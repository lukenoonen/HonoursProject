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
	TestCases                   testCases
)
	: _name( std::move( name ) ),
	  _graphParser( std::move( graphParser ) ),
	  _authorityBuilder( std::move( authorityBuilder ) ),
	  _pathSolverBuilders( std::move( pathSolverBuilders ) ),
	  _testCases( std::move( testCases ) ),
	  _testCaseMap( mapTestCases( _testCases ) )
{

}

bool TestInst::run() const
{
	Timer<std::milli> instTimer;
	size_t failures = 0;

	g_logger.debug( "Running TestInst {}\n", _name );
	g_logger.log( "TestInst {}\n", _name );

	g_logger.debug( "Parsing graph...\n" );
	Ptr<WeightedGraph> graph = _graphParser->create();

	g_logger.debug( "Building path solvers...\n" );

	Ptr<PathSolver> authority = _authorityBuilder->create( *graph );
	Vec<Ptr<PathSolver>> pathSolvers;
	pathSolvers.reserve( _pathSolverBuilders.size() );
	for (const Ptr<PathSolverBuilder>& pathSolverBuilder : _pathSolverBuilders)
	{
		pathSolvers.emplace_back( pathSolverBuilder->create( *graph ) );
	}

	instTimer.start();

	for (const auto& testCase : _testCases)
	{
		failures += testCase->run( *graph, authority.get(), pathSolvers );
	}

	instTimer.stop();

	g_logger.log( "{:.4f}\t{}\n", instTimer.duration(), failures );
	g_logger.debug( "Finished TestInst {}\n", _name );

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

	JSON_FABRICATE(
		std::move( name ),
		std::move( graph ),
		std::move( authority ),
		std::move( pathSolvers ),
		std::move( testCases )
	)

JSON_END()