#include "TestSuite.h"
#include "JSON.h"

namespace
{
	Map<Str, const TestInst*> mapTestInsts( const Vec<TestInst>& testInsts )
	{
		Map<Str, const TestInst*> result;
		for (auto& testInst : testInsts)
		{
			result[testInst.name()] = &testInst;
		}
		return result;
	}
}

TestSuite::TestSuite( Vec<TestInst> testInsts )
	: _testInsts( std::move( testInsts ) ),
	  _testInstMap( mapTestInsts( _testInsts ) )
{

}

bool TestSuite::run( Str testinst, Str testcase ) const
{
	g_logger.debug( "Running test case {} in test inst {}", testcase, testinst );

	if (const auto search = _testInstMap.find( testinst ); search != _testInstMap.end())
	{
		return search->second->run( testcase );
	}

	return false;
}

bool TestSuite::run( Str testinst ) const
{
	g_logger.debug( "Running all test cases in test inst {}...\n", testinst );

	if (const auto search = _testInstMap.find( testinst ); search != _testInstMap.end())
	{
		return search->second->run();
	}

	return false;
}

bool TestSuite::run() const
{
	g_logger.debug( "Running all test cases...\n" );

	bool result = true;

	for (const auto& testInst : _testInsts)
	{
		result = result && testInst.run();
	}

	return result;
}

JSON_BEGIN( TestSuite )

	JSON_ARG( Vec<TestInst>, tests )

	JSON_FABRICATE( std::move( tests ) )

JSON_END()