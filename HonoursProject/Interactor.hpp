#ifndef INTERACTOR_H
#define INTERACTOR_H

#include "BaseGraph.hpp"
#include "Util.hpp"

#include "GraphParser.hpp"
#include "PathSolverBuilder.hpp"
#include "WeightedGraph.hpp"

class Interactor
{
private:
	USING_GRAPH(WeightedGraph);

public:
	class PathSolverInfo;

public:
	Interactor() = default;
	Interactor(
		Ptr<GraphParser>    graphParser,
		Vec<PathSolverInfo> infos
	);

	void start();

private:
	void build(const Vec<Str>& tokens);
	void erase(const Vec<Str>& tokens);
	void clear(const Vec<Str>& tokens);
	void random(const Vec<Str>& tokens);
	void query(const Vec<Str>& tokens);

	void executeQuery(const PathSolverInfo& info, Vertex s, Vertex t, bool verbose);
	void executeQuery(const PathSolverInfo& info, Vertex s, Vec<Vertex> ts, bool verbose);

private:
	Ptr<GraphParser> _graphParser;

	Vec<PathSolverInfo>             _infos;
	Map<Str, const PathSolverInfo*> _infomap;

	Ptr<WeightedGraph> _graph;
	Map<Str, Ptr<PathSolver>> _pathSolverMap;
};

JSON_CREATE(Interactor)

class Interactor::PathSolverInfo
{
public:
	PathSolverInfo() = default;
	PathSolverInfo(Str name, Ptr<PathSolverBuilder> builder);

	[[nodiscard]] const Str&         name() const;
	[[nodiscard]] PathSolverBuilder* builder() const;

private:
	Str                    _name;
	Ptr<PathSolverBuilder> _builder;
};

JSON_CREATE(Interactor::PathSolverInfo)

#endif // INTERACTOR_H