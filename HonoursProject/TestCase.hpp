#ifndef TESTCASE_H
#define TESTCASE_H

#include "Util.hpp"

#include "Factory.hpp"
#include "JSON.hpp"

#include "PathSolver.hpp"

#include "Logger.hpp"

class TestCase
{
public:
	class Endpoints;
	using RunArg = Pair<Endpoints, Vec<double>>;

protected:
	USING_GRAPH(WeightedGraph);

	class Result;
	class Results;

protected:
	explicit TestCase(Str name);

public:
	virtual ~TestCase()                  = default;
	TestCase(const TestCase&)            = default;
	TestCase& operator=(const TestCase&) = default;
	TestCase(TestCase&&)                 = default;
	TestCase& operator=(TestCase&&)      = default;

	[[nodiscard]] const Str& name() const;

	Vec<RunArg> runAuthority(
		const WeightedGraph& graph,
		const PathSolver*    authority,
		const Logger&        results
	) const;

	size_t run(
		const Vec<RunArg>& args,
		const PathSolver*  pathSolver,
		const Logger&      results
	) const;

protected:
	[[nodiscard]] virtual Vec<Endpoints> endpoints(
		const WeightedGraph& graph
	) const = 0;

private:
	Str _name;
};

class TestCase::Endpoints
{
public:
	Endpoints();
	Endpoints(Vertex u, Vec<Vertex> vs);

	[[nodiscard]] Vertex             u() const;
	[[nodiscard]] const Vec<Vertex>& vs() const;

private:
	Vertex      _u;
	Vec<Vertex> _vs;
};

JSON_CREATE(TestCase::Endpoints)

class TestCase::Result
{
public:
	Result(double distance, double duration);

	[[nodiscard]] double distance() const;
	[[nodiscard]] double duration() const;

private:
	double _distance;
	double _duration;
};

class TestCase::Results
{
public:
	Results();

private:
	Result      _authority;
	Vec<Result> _pathSolvers;
};

JSON_CREATE_FACTORY(TestCase)

FACTORY_CREATE_BASE_JSON(TestCase)

class AllTestCase : public TestCase
{
public:
	explicit AllTestCase(Str name);

	[[nodiscard]] Vec<Endpoints> endpoints(
		const WeightedGraph& graph
	) const final;
};

FACTORY_CREATE_JSON(AllTestCase, TestCase)

class ListTestCase : public TestCase
{
public:
	ListTestCase(Vec<Endpoints> endpoints, Str name);

	[[nodiscard]] Vec<Endpoints> endpoints(
		const WeightedGraph& graph
	) const final;

private:
	Vec<Endpoints> _endpoints;
};

FACTORY_CREATE_JSON(ListTestCase, TestCase)

class RandomTestCase : public TestCase
{
public:
	RandomTestCase(size_t trials, Str name);

	[[nodiscard]] Vec<Endpoints> endpoints(
		const WeightedGraph& graph
	) const final;

private:
	size_t _trials;
};

FACTORY_CREATE_JSON(RandomTestCase, TestCase)

class RandomOneToManyTestCase : public TestCase
{
public:
	RandomOneToManyTestCase(
		size_t trials,
		size_t start,
		size_t factor,
		size_t count,
		Str    name
	);

	[[nodiscard]] Vec<Endpoints> endpoints(
		const WeightedGraph& graph
	) const final;

private:
	size_t _trials;
	size_t _start;
	size_t _factor;
	size_t _count;
};

FACTORY_CREATE_JSON(RandomOneToManyTestCase, TestCase)

#endif  // TESTCASE_H