#ifndef TESTINST_H
#define TESTINST_H

#include "Util.h"
#include "JSON.h"

#include "WeightedGraph.h"
#include "PathSolver.h"
#include "TestCase.h"

class TestInst
{
public:
	using TestCases   = Vec<Ptr<TestCase>>;
	using TestCaseMap = Map<Str, const TestCase*>;

public:
	TestInst() = default;
	TestInst(
		Str                  name,
		Ptr<WeightedGraph>   graph,
		Ptr<PathSolver>      authority,
		Vec<Ptr<PathSolver>> pathSolvers,
		TestCases            testCases
	);

	bool run( Str testcase ) const;
	bool run() const;

	const Str& name() const;

private:
	Str _name;

	Ptr<WeightedGraph>   _graph;
	Ptr<PathSolver>      _authority;
	Vec<Ptr<PathSolver>> _pathSolvers;

	TestCases   _testCases;
	TestCaseMap _testCaseMap;
};

JSON_CREATE( TestInst )

#endif // TESTINST_H