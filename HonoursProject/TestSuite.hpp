#ifndef TESTSUITE_H
#define TESTSUITE_H

#include "Util.hpp"

#include "TestInst.hpp"

class TestSuite
{
public:
	TestSuite() = default;
	explicit TestSuite(Vec<TestInst> testInsts);

	bool run(Str testinst) const;
	bool run() const;

private:
	Vec<TestInst> _testInsts;

	Map<Str, const TestInst*> _testInstMap;
};

JSON_CREATE(TestSuite)

#endif  // TESTSUITE_H