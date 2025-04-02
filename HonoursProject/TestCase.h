#ifndef TESTCASE_H
#define TESTCASE_H

#include "Util.h"

#include "JSON.h"
#include "Factory.h"

#include "PathSolver.h"

#include "Logger.h"

class TestCase
{
public:

	class Endpoints;
	using RunArg = Pair<Endpoints, Vec<double>>;

protected:
	USING_GRAPH( WeightedGraph );

	class Result;
	class Results;

protected:
	TestCase( Str name );

public:
	virtual ~TestCase() = default;

	const Str& name() const;

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
	virtual Vec<Endpoints> endpoints( const WeightedGraph& graph ) const = 0;

private:
	const Str _name;
};

class TestCase::Endpoints
{
public:
	Endpoints() = default;
	Endpoints( Vertex u, Vec<Vertex> vs );

	Vertex u() const;
	const Vec<Vertex>& vs() const;

private:
	Vertex      _u;
	Vec<Vertex> _vs;
};

JSON_CREATE( TestCase::Endpoints )

class TestCase::Result
{
public:
	Result( double distance, double duration );

	double distance() const;
	double duration() const;

private:
	const double _distance;
	const double _duration;
};

class TestCase::Results
{
public:
	Results();

private:
	Result      _authority;
	Vec<Result> _pathSolvers;
};

JSON_CREATE_FACTORY( TestCase )

FACTORY_CREATE_BASE_JSON( TestCase )

class AllTestCase : public TestCase
{
public:
	AllTestCase( Str name );

	Vec<Endpoints> endpoints( const WeightedGraph& graph ) const final;
};

FACTORY_CREATE_JSON( AllTestCase, TestCase )

class ListTestCase : public TestCase
{
public:
	ListTestCase( Vec<Endpoints> endpoints, Str name );

	Vec<Endpoints> endpoints( const WeightedGraph& graph ) const final;

private:
	const Vec<Endpoints> _endpoints;
};

FACTORY_CREATE_JSON( ListTestCase, TestCase )

class RandomTestCase : public TestCase
{
public:
	RandomTestCase( size_t trials, Str name );

	Vec<Endpoints> endpoints( const WeightedGraph& graph ) const final;

private:
	const size_t _trials;
};

FACTORY_CREATE_JSON( RandomTestCase, TestCase )

class RandomOneToManyTestCase : public TestCase
{
public:
	RandomOneToManyTestCase( size_t trials, size_t start, size_t factor, size_t count, Str name );

	Vec<Endpoints> endpoints( const WeightedGraph& graph ) const final;

private:
	const size_t _trials;
	const size_t _start;
	const size_t _factor;
	const size_t _count;
};

FACTORY_CREATE_JSON( RandomTestCase, TestCase )

#endif // TESTCASE_H