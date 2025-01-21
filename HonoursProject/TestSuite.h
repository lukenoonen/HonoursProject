#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "Util.h"

#include "TestInst.h"
#include "Logger.h"

class TestSuite
{
public:
	TestSuite() = default;
	TestSuite( Vec<TestInst> testInsts );

	bool run( Str testinst, Str testcase ) const;
	bool run( Str testinst ) const;
	bool run() const;

private:
	Vec<TestInst> _testInsts;

	Map<Str, const TestInst*> _testInstMap;
};

JSON_CREATE( TestSuite )

#endif // TESTSUITE_H