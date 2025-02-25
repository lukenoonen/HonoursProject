#include "Logger.h"
#include "Profiler.h"

#define BUILD_TIMES_FILE "build_times.dat"

template<class T>
inline CachedPathSolverBuilder<T>::CachedPathSolverBuilder( FilePath filepath )
	: _filepath( std::move( filepath ) )
{

}

template<class T>
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::createInternal( const WeightedGraph& graph ) const
{
	auto result = load();
	if (result) { return result; }
	g_logger.debug( "Failed to load path solver from {}!\n", _filepath.string() );
	return build( graph );
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
inline Ptr<PathSolver> CachedPathSolverBuilder<T>::build( const WeightedGraph& graph ) const
{
	g_logger.debug( "Building path solver...\n" );
	Timer buildTimer;
	buildTimer.start();
	Ptr<T> result = buildInternal( graph );
	buildTimer.stop();

	Map<Str, double> buildTimes;
	deserialize( BUILD_TIMES_FILE, buildTimes );
	buildTimes[_filepath.string()] = buildTimer.duration();
	serialize( BUILD_TIMES_FILE, buildTimes );

	if (!_filepath.empty()) { serialize( _filepath, *result ); }
	return result;
}