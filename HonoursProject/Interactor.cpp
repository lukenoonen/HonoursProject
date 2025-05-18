#include "Interactor.hpp"

#include "PathSolverBuilder.hpp"
#include "Profiler.hpp"
#include "PathSolver.hpp"

#include "Logger.hpp"

#include <cstring>
#include <format>
#include <random>

#include <readline/readline.h>
#include <readline/history.h>

namespace
{
	Map<Str, const Interactor::PathSolverInfo*> getInfomap(
		const Vec<Interactor::PathSolverInfo>& infos
	)
	{
		Map<Str, const Interactor::PathSolverInfo*> result;
		result.reserve(infos.size());
		for (const auto& info : infos)
		{
			result[info.name()] = &info;
		}
		return result;
	}

	Map<Str, Ptr<PathSolver>> getPathSolverMap(
		const Vec<Interactor::PathSolverInfo>& infos
	)
	{
		Map<Str, Ptr<PathSolver>> result;
		result.reserve(infos.size());
		for (const auto& info : infos)
		{
			if (result.contains(info.name()))
			{
				g_logger.log("Warning: path solver with name '{}' already present\n", info.name());
			}
			
			if (info.name() == "graph")
			{
				g_logger.log("Warning: path solver using reserved name 'graph'\n");
			}
			else if (info.name() == "solvers")
			{
				g_logger.log("Warning: path solver using reserved name 'solvers'\n");
			}
			else if (info.name() == "all")
			{
				g_logger.log("Warning: path solver using reserved name 'all'\n");
			}

			result[info.name()] = nullptr;
		}
		return result;
	}

	Vec<Str> tokenize(const char* str, char delim)
	{
		std::stringstream ss(str);
		Vec<Str> result;
		Str line;
		while (std::getline(ss, line, delim))
		{
			if (line.empty()) { continue; }
			result.emplace_back(std::move(line));
		}
		return result;
	}

	template <class T>
	T convert(const Str& str)
	{
		T result;

		std::stringstream stream(str);
		stream >> result;
		
		return result;
	}

	template <class T>
	Vec<T> convertv(const Str& str)
	{
		const Vec<Str> tokens = tokenize(str.c_str(), ',');

		Vec<T> result;
		result.reserve(tokens.size());

		for (const Str& str : tokens)
		{
			result.push_back(convert<T>(str));
		}
		
		return result;
	}
}  // namespace

Interactor::Interactor(
	Ptr<GraphParser>    graphParser,
	Vec<PathSolverInfo> infos
)
	: _graphParser(std::move(graphParser)),
	  _infos(std::move(infos)),
	  _infomap(getInfomap(_infos)),
	  _graph(nullptr),
	  _pathSolverMap(getPathSolverMap(_infos))
{
}

namespace
{
	template <class T> 
	Str stringify(const Vec<T>& vec)
	{
		std::stringstream ss;
		ss << '[' << vec.front();
		for (size_t i = 1; i < vec.size(); i++)
		{
			ss << ", " << vec[i];
		}
		ss << ']';
		return ss.str();
	}

	char* character_name_generator(const char *text, int state)
	{
		static std::array<const char*, 13> commands = {
			"build",
			"erase",
			"clear",
			"random",
			"query",
			"exit",
			"help",
			"all",
			"graph",
			"solvers",
			"recache",
			"verbose",
			nullptr
		};

		static size_t list_index{};
		static size_t len{};
		const char *name{};

		if (state == 0)
		{
			list_index = 0;
			len = strlen(text);
		}

		while ((name = commands[list_index++]) != nullptr)
		{
			if (strncmp(name, text, len) == 0)
			{
				return strdup(name);
			}
		}

		return nullptr;
	}

	char** character_name_completion(const char *text, int /*start*/, int /*end*/)
	{
		rl_attempted_completion_over = 1;
		return rl_completion_matches(text, character_name_generator);
	}
}  // namespace

void Interactor::start()
{
	g_logger.log("Starting interactive mode...\n");

    rl_attempted_completion_function = character_name_completion;

	Ptr<char[]> prevLine = nullptr;

	while (true)
	{
		Ptr<char[]> line(readline("> "));
		const Vec<Str> tokens = tokenize(line.get(), ' ');

		if (tokens.empty()) { continue; }

		if (prevLine.get() == nullptr || std::strcmp(prevLine.get(), line.get()) != 0)
		{
			add_history(line.get());
			prevLine = std::move(line);
		}

		if (tokens[0] == "help")
		{
			g_logger.log("build (all|graph|solvers|*) [recache]       Build the graph and path solvers. Optionally recache results.\n");
			g_logger.log("erase (all|graph|solvers|*)                 Erase the graph and path solvers.\n");
			g_logger.log("clear (all|graph|solvers|*)                 Clear the cache of the graph and path solvers.\n");
			g_logger.log("random (all|*) n [m] [verbose]              Queries path solvers randomly n times to m vertices.\n");
			g_logger.log("query (all|*) s (t|t1,t2,...,tn) [verbose]  Queries path solvers from s to t(s).\n");
		}
		else if (tokens[0] == "build")
		{
			build(tokens);
		}
		else if (tokens[0] == "erase")
		{
			erase(tokens);
		}
		else if (tokens[0] == "clear")
		{
			clear(tokens);
		}
		else if (tokens[0] == "random")
		{
			random(tokens);
		}
		else if (tokens[0] == "query")
		{
			query(tokens);
		}
		else if (tokens[0] == "exit")
		{
			break;
		}
		else
		{
			g_logger.log("Unknown command '{}', enter 'help' for interactive mode documentation\n", tokens[0]);
		}
	}
}

void Interactor::build(const Vec<Str>& tokens)
{
	if (tokens.size() < 2)
	{
		g_logger.log("Error: 'build' requires at least 2 arguments, {} provided\n", tokens.size());
		return;
	}

	bool recache = false;
	if (tokens.size() >= 3)
	{
		if (tokens[2] == "recache")
		{
			recache = true;
		}
	}

	if (tokens[1] == "all")
	{
		if (recache) { _graphParser->clearCache(); }
		_graph = _graphParser->create();

		for (const auto& info : _infos)
		{
			if (recache) { info.builder()->clearCache(); }
			_pathSolverMap[info.name()] = info.builder()->create(*_graph);
		}

		return;
	}

	if (tokens[1] == "graph")
	{
		if (recache) { _graphParser->clearCache(); }
		_graph = _graphParser->create();
		return;
	}

	if (_graph == nullptr)
	{
		g_logger.log("Error: unable to build solvers before first building graph\n");
		return;
	}

	if (tokens[1] == "solvers")
	{
		for (const auto& info : _infos)
		{
			if (recache) { info.builder()->clearCache(); }
			_pathSolverMap[info.name()] = info.builder()->create(*_graph);
		}

		return;
	}

	if (const auto search = _infomap.find(tokens[1]); search != _infomap.end())
	{
		if (recache) { search->second->builder()->clearCache(); }
		_pathSolverMap[tokens[1]] = search->second->builder()->create(*_graph);
	}
	else
	{
		g_logger.log("Error: unable to find path solver builder '{}'\n", tokens[1]);
	}
}

void Interactor::erase(const Vec<Str>& tokens)
{
	if (tokens.size() < 2)
	{
		g_logger.log("Error: 'erase' requires at least 2 arguments, {} provided\n", tokens.size());
		return;
	}

	if (tokens[1] == "all")
	{
		_graph = nullptr;

		for (const auto& info : _infos)
		{
			_pathSolverMap[info.name()] = nullptr;
		}

		return;
	}

	if (tokens[1] == "graph")
	{
		_graph = nullptr;
		return;
	}

	if (tokens[1] == "solvers")
	{
		for (const auto& info : _infos)
		{
			_pathSolverMap[info.name()] = nullptr;
		}

		return;
	}

	if (_infomap.contains(tokens[1]))
	{
		_pathSolverMap[tokens[1]] = nullptr;
	}
	else
	{
		g_logger.log("Error: unable to find path solver '{}'\n", tokens[1]);
	}
}

void Interactor::clear(const Vec<Str>& tokens)
{
	if (tokens.size() < 2)
	{
		g_logger.log("Error: 'clear' requires at least 2 arguments, {} provided\n", tokens.size());
		return;
	}

	if (tokens[1] == "all")
	{
		_graphParser->clearCache();

		for (const auto& info : _infos)
		{
			info.builder()->clearCache();
		}

		return;
	}

	if (tokens[1] == "graph")
	{
		_graphParser->clearCache();
		return;
	}

	if (tokens[1] == "solvers")
	{
		for (const auto& info : _infos)
		{
			info.builder()->clearCache();
		}

		return;
	}

	if (const auto search = _infomap.find(tokens[1]); search != _infomap.end())
	{
		search->second->builder()->clearCache();
	}
	else
	{
		g_logger.log("Error: unable to find path solver '{}'\n", tokens[1]);
	}
}

void Interactor::random(const Vec<Str>& tokens)
{
	if (tokens.size() < 3)
	{
		g_logger.log("Error: 'random' requires at least 3 arguments, {} provided\n", tokens.size());
		return;
	}

	if (_graph == nullptr)
	{
		g_logger.log("Error: unable to query solvers before first building graph\n");
		return;
	}

	bool verbose = false;
	if (tokens.size() >= 5)
	{
		if (tokens[4] == "verbose")
		{
			verbose = true;
		}
	}

	static std::mt19937                   rng(std::random_device{}());
	std::uniform_int_distribution<Vertex> dist(0, _graph->numVertices() - 1);

	const auto n = convert<size_t>(tokens[2]);
	const auto m = tokens.size() >= 4 ? convert<size_t>(tokens[3]) : 1;

	Vec<Pair<Vertex, Vec<Vertex>>> queries(n);
	for (auto& query : queries)
	{
		query.first = dist(rng);
		for (size_t i = 0; i < m; i++)
		{
			query.second.push_back(dist(rng));
		}
	}

	if (tokens[1] == "all")
	{
		for (const auto& info : _infos)
		{
			Timer timer;
			timer.start();

			for (const auto& query : queries)
			{
				if (query.second.size() == 1)
				{
					executeQuery(info, query.first, query.second.front(), verbose);
				}
				else
				{
					executeQuery(info, query.first, query.second, verbose);
				}
			}

			timer.stop();
			g_logger.log("Queries took {} seconds to execute on path solver '{}'\n", timer.duration(), info.name());
		}
		return;
	}

	if (const auto search = _infomap.find(tokens[1]); search != _infomap.end())
	{
		Timer timer;
		timer.start();

		for (const auto& query : queries)
		{
			if (query.second.size() == 1)
			{
				executeQuery(*search->second, query.first, query.second.front(), verbose);
			}
			else
			{
				executeQuery(*search->second, query.first, query.second, verbose);
			}
		}

		timer.stop();
		g_logger.log("Queries took {} seconds to execute on path solver '{}'\n", timer.duration(), tokens[1]);
	}
	else
	{
		g_logger.log("Error: unable to find path solver '{}'\n", tokens[1]);
	}
}

void Interactor::query(const Vec<Str>& tokens)
{
	if (tokens.size() < 4)
	{
		g_logger.log("Error: 'query' requires at least 4 arguments, {} provided\n", tokens.size());
		return;
	}

	if (_graph == nullptr)
	{
		g_logger.log("Error: unable to query solvers before first building graph\n");
		return;
	}

	bool verbose = false;
	if (tokens.size() >= 5)
	{
		if (tokens[4] == "verbose")
		{
			verbose = true;
		}
	}

	const auto s  = convert<Vertex>(tokens[2]);
	const auto ts = convertv<Vertex>(tokens[3]);

	if (tokens[1] == "all")
	{
		for (const auto& info : _infos)
		{
			Timer timer;
			timer.start();

			if (ts.size() == 1)
			{
				executeQuery(info, s, ts.front(), verbose);
			}
			else
			{
				executeQuery(info, s, ts, verbose);
			}

			timer.stop();
			g_logger.log("Query took {} seconds to execute on path solver '{}'\n", timer.duration(), info.name());
		}
		return;
	}

	if (const auto search = _infomap.find(tokens[1]); search != _infomap.end())
	{
		Timer timer;
		timer.start();

		if (ts.size() == 1)
		{
			executeQuery(*search->second, s, ts.front(), verbose);
		}
		else
		{
			executeQuery(*search->second, s, ts, verbose);
		}

		timer.stop();
		g_logger.log("Query took {} seconds to execute on path solver '{}'\n", timer.duration(), tokens[1]);
	}
	else
	{
		g_logger.log("Error: unable to find path solver '{}'\n", tokens[1]);
	}
}

void Interactor::executeQuery(const PathSolverInfo& info, Vertex s, Vertex t, bool verbose)
{
	PathSolver* pathSolver = _pathSolverMap.find(info.name())->second.get();
	if (pathSolver == nullptr)
	{
		g_logger.log("Warning: unable to query path solver '{}' before building it\n", info.name());
		return;
	}
	Timer timer;
	timer.start();
	const double distance = pathSolver->distance(s, t);
	timer.stop();

	if (verbose)
	{
		g_logger.log(
			"Query from {} to {} in path solver '{}' returned distance {} and took {} seconds\n",
			s,
			t,
			info.name(),
			distance,
			timer.duration()
		);
	}
}

void Interactor::executeQuery(const PathSolverInfo& info, Vertex s, Vec<Vertex> ts, bool verbose)
{
	PathSolver* pathSolver = _pathSolverMap.find(info.name())->second.get();
	if (pathSolver == nullptr)
	{
		g_logger.log("Warning: unable to query path solver '{}' before building it\n", info.name());
		return;
	}

	Timer timer;
	timer.start();
	const Vec<double> distances = pathSolver->distances(s, ts);
	timer.stop();

	if (verbose)
	{
		g_logger.log(
			"Query from {} to {} in path solver '{}' returned distance {} and took {} seconds\n",
			s,
			stringify(ts),
			info.name(),
			stringify(distances),
			timer.duration()
		);
	}
}

JSON_BEGIN(Interactor)

	JSON_ARG(Ptr<GraphParser>, graph)
	JSON_ARG(Vec<Interactor::PathSolverInfo>, pathSolvers)

	JSON_FABRICATE(std::move(graph), std::move(pathSolvers))

JSON_END()

Interactor::PathSolverInfo::PathSolverInfo(Str name, Ptr<PathSolverBuilder> builder)
	: _name(std::move(name)),
	  _builder(std::move(builder))
{
}

const Str& Interactor::PathSolverInfo::name() const
{
	return _name;
}

PathSolverBuilder* Interactor::PathSolverInfo::builder() const
{
	return _builder.get();
}

JSON_BEGIN(Interactor::PathSolverInfo)

	JSON_ARG(Str, name)
	JSON_ARG(Ptr<PathSolverBuilder>, pathSolver)

	JSON_FABRICATE(std::move(name), std::move(pathSolver))

JSON_END()