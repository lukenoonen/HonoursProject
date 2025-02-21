#include "Logger.h"
#include "Profiler.h"

#define BUILD_TIMES_FILE "build_times.dat"

template<class T>
inline CachedPathSolverBuilder<T>::CachedPathSolverBuilder( FilePath filepath, const WeightedGraph& graph )
	: PathSolverBuilder( graph ),
	  _filepath( std::move( filepath ) )
{

}

template<class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::createInternal() const
{
	auto result = load();
	if (result) { return result; }
	g_logger.debug( "Failed to load path solver from {}!\n", _filepath.string() );
	return build();
}

template<class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::load() const
{
	g_logger.debug( "Loading path solver...\n" );
	Ptr<T> result = std::make_unique<T>();
	if (!_filepath.empty() && deserialize( _filepath, *result )) { return result; }
	return nullptr;
}

template<class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::build() const
{
	g_logger.debug( "Building path solver...\n" );
	Timer buildTimer;
	buildTimer.start();
	Ptr<T> result = buildInternal();
	buildTimer.stop();

	Map<Str, double> buildTimes;
	deserialize( BUILD_TIMES_FILE, buildTimes );
	buildTimes[_filepath.string()] = buildTimer.duration();
	serialize( BUILD_TIMES_FILE, buildTimes );

	if (!_filepath.empty()) { serialize( _filepath, *result ); }
	return result;
}