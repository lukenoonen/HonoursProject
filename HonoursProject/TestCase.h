#ifndef TESTCASE_H
#define TESTCASE_H

#include "Util.h"

#include "JSON.h"
#include "Factory.h"

#include "PathSolver.h"

class TestCase
{
public:
	class Endpoints;

protected:
	class Result;
	class Results;

protected:
	TestCase( Str name );

public:
	size_t run(
		const WeightedGraph&        graph,
		const PathSolver*           authority,
		const Vec<Ptr<PathSolver>>& pathSolvers
	) const;

	const Str& name() const;

private:
	double run(
		const Endpoints&  endpoints,
		const PathSolver* pathSolver,
		double            authorityDistance = -1.0
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
	Endpoints( size_t u, size_t v );

	size_t u() const;
	size_t v() const;

private:
	size_t _u;
	size_t _v;
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

#endif // TESTCASE_H