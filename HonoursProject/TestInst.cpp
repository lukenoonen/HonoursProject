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
	Str                  name,
	Ptr<WeightedGraph>   graph,
	Ptr<PathSolver>      authority,
	Vec<Ptr<PathSolver>> pathSolvers,
	TestCases            testCases
)
	: _name( std::move( name ) ),
	  _graph( std::move( graph ) ),
	  _pathSolvers( std::move( pathSolvers ) ),
	  _authority( std::move( authority ) ),
	  _testCases( std::move( testCases ) ),
	  _testCaseMap( mapTestCases( _testCases ) )
{

}

bool TestInst::run( Str testcase ) const
{
	if (const auto search = _testCaseMap.find( testcase ); search != _testCaseMap.end())
	{
		search->second->run( *_graph, _authority.get(), _pathSolvers );
		return true;
	}

	g_logger.debug( "Warning: Unable to find TestCase {} in TestInst {}", testcase, _name );
	return false;
}

bool TestInst::run() const
{
	Timer instTimer;
	size_t failures = 0;

	g_logger.debug( "Running TestInst {}\n", _name );
	g_logger.log( "TestInst {}\n", _name );

	instTimer.start();

	for (const auto& testCase : _testCases)
	{
		failures += testCase->run( *_graph, _authority.get(), _pathSolvers);
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
	JSON_ARG( Ptr<WeightedGraph>, graph )
	JSON_ARG( Ptr<PathSolver>, authority, *graph )
	JSON_ARG( Vec<Ptr<PathSolver>>, pathSolvers, *graph )
	JSON_ARG( TestInst::TestCases, testCases )

	JSON_FABRICATE(
		std::move( name ),
		std::move( graph ),
		std::move( authority ),
		std::move( pathSolvers ),
		std::move( testCases )
	)

JSON_END()