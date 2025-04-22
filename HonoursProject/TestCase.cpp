#include "TestCase.hpp"
#include "Logger.hpp"
#include "Profiler.hpp"

#include <random>

TestCase::TestCase(Str name)
	: _name(std::move(name))
{
}

const Str& TestCase::name() const
{
	return _name;
}

Vec<TestCase::RunArg> TestCase::runAuthority(
	const WeightedGraph& graph,
	const PathSolver*    authority,
	const Logger&        results
) const
{
	g_logger.log("Running TestCase {} (authority)\n", _name);
	results.log("# TestCase {} (authority):\n", _name);

	Vec<Endpoints> eps = endpoints(graph);

	Vec<TestCase::RunArg> result;
	result.reserve(eps.size());

	double totalTime = 0.0;

	size_t count = 0;
	for (Endpoints& ep : eps)
	{
		count++;

		Timer<std::milli> timer;
		timer.start();
		Vec<double> distances = authority->distances(ep.u(), ep.vs());
		timer.stop();
		result.emplace_back(std::move(ep), std::move(distances));

		results.log("{:.4f}\n", timer.duration());
		totalTime += timer.duration();
	}

	results.log("{:.4f}\n", totalTime);

	return result;
}

size_t TestCase::run(
	const Vec<RunArg>& args,
	const PathSolver*  pathSolver,
	const Logger&      results
) const
{
	g_logger.log("Running TestCase {} (path solver)\n", _name);
	results.log("# TestCase {} (path solver):\n", _name);

	size_t failures  = 0;
	double totalTime = 0.0;

	size_t count = 0;
	for (const auto& [ep, ds] : args)
	{
		count++;
		
		const Vec<Vertex>& vs = ep.vs();
		Timer<std::milli>  timer;
		timer.start();
		const Vec<double> distances = pathSolver->distances(ep.u(), vs);
		timer.stop();

		for (size_t i = 0; i < distances.size(); i++)
		{
			const double distance = distances[i];
			const double d        = ds[i];
			if (std::abs(d - distance) >= 1.0)
			{
				g_logger.log(
					"Failed {} to {}: {} vs {}\n", ep.u(), vs[i], d, distance
				);
				failures++;
			}
		}

		results.log("{:.4f}\n", timer.duration());
		totalTime += timer.duration();
	}

	results.log("{:.4f}\n", totalTime);

	return failures;
}

JSON_DEFINE_FACTORY(TestCase)

TestCase::Endpoints::Endpoints()
	: _u{}
{
}

TestCase::Endpoints::Endpoints(Vertex u, Vec<Vertex> vs)
	: _u(u),
	  _vs(std::move(vs))
{
}

size_t TestCase::Endpoints::u() const
{
	return _u;
}

const Vec<size_t>& TestCase::Endpoints::vs() const
{
	return _vs;
}

JSON_BEGIN(TestCase::Endpoints)

	JSON_ARG(size_t, u)
	JSON_ARG(size_t, v)

	JSON_FABRICATE(u, {v})

JSON_END()

TestCase::Result::Result(double distance, double duration)
	: _distance(distance),
	  _duration(duration)
{
}

double TestCase::Result::distance() const
{
	return _distance;
}

double TestCase::Result::duration() const
{
	return _duration;
}

AllTestCase::AllTestCase(Str name)
	: TestCase(std::move(name))
{
}

Vec<TestCase::Endpoints> AllTestCase::endpoints(const WeightedGraph& g) const
{
	Vec<TestCase::Endpoints> result;
	for (Vertex v = 0; v < g.numVertices(); v++)
	{
		for (Vertex u = v + 1; u < g.numVertices(); u++)
		{
			result.emplace_back(v, Vec{u});
		}
	}
	return result;
}

FACTORY_BEGIN_JSON("all", AllTestCase, TestCase)

	JSON_ARG(Str, name)

	FACTORY_FABRICATE(std::move(name))

FACTORY_END()

ListTestCase::ListTestCase(Vec<Endpoints> endpoints, Str name)
	: TestCase(std::move(name)),
	  _endpoints(std::move(endpoints))
{
}

Vec<TestCase::Endpoints> ListTestCase::endpoints(const WeightedGraph& /*unused*/) const
{
	return _endpoints;
}

FACTORY_BEGIN_JSON("list", ListTestCase, TestCase)

	JSON_ARG(Vec<TestCase::Endpoints>, endpoints)
	JSON_ARG(Str, name)

	FACTORY_FABRICATE(std::move(endpoints), std::move(name))

FACTORY_END()

RandomTestCase::RandomTestCase(size_t trials, Str name)
	: TestCase(std::move(name)),
	  _trials(trials)
{
}

Vec<TestCase::Endpoints> RandomTestCase::endpoints(const WeightedGraph& graph
) const
{
	static std::mt19937                   rng(std::random_device{}());
	std::uniform_int_distribution<Vertex> dist(0, graph.numVertices() - 1);

	Vec<Endpoints> endpoints;
	endpoints.reserve(_trials);
	for (size_t i = 0; i < _trials; i++)
	{
		endpoints.emplace_back(dist(rng), Vec{dist(rng)});
	}

	return endpoints;
}

FACTORY_BEGIN_JSON("random", RandomTestCase, TestCase)

	JSON_ARG(size_t, trials)
	JSON_ARG(Str, name)

	FACTORY_FABRICATE(trials, std::move(name))

FACTORY_END()

RandomOneToManyTestCase::RandomOneToManyTestCase(
	size_t trials,
	size_t start,
	size_t factor,
	size_t count,
	Str    name
)
	: TestCase(std::move(name)),
	  _trials(trials),
	  _start(start),
	  _factor(factor),
	  _count(count)
{
}

Vec<TestCase::Endpoints> RandomOneToManyTestCase::endpoints(
	const WeightedGraph& graph
) const
{
	static std::mt19937                   rng(std::random_device{}());
	std::uniform_int_distribution<Vertex> dist(0, graph.numVertices() - 1);

	Vec<Endpoints> endpoints;
	endpoints.reserve(_trials * _count);
	size_t generator = _start;
	for (size_t i = 0; i < _count; i++)
	{
		for (size_t i = 0; i < _trials; i++)
		{
			Vec<Vertex> vs;
			vs.reserve(generator);
			for (size_t j = 0; j < generator; j++)
			{
				vs.emplace_back(dist(rng));
			}
			endpoints.emplace_back(dist(rng), std::move(vs));
		}
		generator *= _factor;
	}

	return endpoints;
}

FACTORY_BEGIN_JSON("random_one_to_many", RandomOneToManyTestCase, TestCase)

	JSON_ARG(size_t, trials)
	JSON_ARG(size_t, start)
	JSON_ARG(size_t, factor)
	JSON_ARG(size_t, count)
	JSON_ARG(Str, name)

FACTORY_FABRICATE(trials, start, factor, count, std::move(name))

FACTORY_END()