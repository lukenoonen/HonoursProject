#ifndef TESTINST_H
#define TESTINST_H

#include "Util.h"
#include "JSON.h"

#include "GraphParser.h"
#include "PathSolverBuilder.h"
#include "TestCase.h"
#include "Logger.h"

class TestInst
{
public:
	using TestCases   = Vec<Ptr<TestCase>>;
	using TestCaseMap = Map<Str, const TestCase*>;

public:
	TestInst() = default;
	TestInst(
		Str                         name,
		Ptr<GraphParser>            graphParser,
		Ptr<PathSolverBuilder>      authorityBuilder,
		Vec<Ptr<PathSolverBuilder>> pathSolverBuilders,
		TestCases                   testCases,
		Logger                      results
	);

	bool run() const;

	const Str& name() const;

private:
	Str _name;

	Ptr<GraphParser>            _graphParser;
	Ptr<PathSolverBuilder>      _authorityBuilder;
	Vec<Ptr<PathSolverBuilder>> _pathSolverBuilders;

	TestCases   _testCases;
	TestCaseMap _testCaseMap;

	Logger _results;
};

JSON_CREATE( TestInst )

#endif // TESTINST_H