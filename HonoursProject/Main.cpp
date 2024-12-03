#include "WeightedGraph.h"
#include "GraphParser.h"
#include "RoadDParser.h"
#include "CIHierarchy.h"

#include <iostream>
#include <random>

double random()
{
	static std::random_device rd;
	static std::mt19937 generator( rd() );
	static std::uniform_real_distribution<> distribution( 0.0, 1.0 );
	return distribution( generator );
}

int main( int argc, const char* argv[] )
{
	/* WeightedGraph wg;
	auto a = wg.addVertex({"a"});
	auto b = wg.addVertex( { "b" } );
	auto c = wg.addVertex( { "c" } );
	auto d = wg.addVertex( { "d" } );
	auto e = wg.addVertex( { "e" } );
	auto f = wg.addVertex( { "f" } );
	auto g = wg.addVertex( { "g" } );
	auto h = wg.addVertex( { "h" } );
	auto i = wg.addVertex( { "i" } );
	auto j = wg.addVertex( { "j" } );
	auto k = wg.addVertex( { "k" } );
	auto l = wg.addVertex( { "l" } );
	auto m = wg.addVertex( { "m" } );

	wg.addEdge( a, b, { 1.0 } );
	wg.addEdge( b, c, { 1.0 } );
	wg.addEdge( b, d, { 1.0 } );
	wg.addEdge( b, e, { 1.0 } );
	wg.addEdge( c, f, { 0.9 } );
	wg.addEdge( d, f, { 1.0 } );
	wg.addEdge( d, g, { 1.0 } );
	wg.addEdge( g, h, { 1.0 } );
	wg.addEdge( h, i, { 1.0 } );
	wg.addEdge( h, j, { 1.0 } );
	wg.addEdge( h, k, { 1.0 } );
	wg.addEdge( k, l, { 1.0 } );
	wg.addEdge( k, m, { 1.0 } );

	std::cout << wg << "\n\n";

	ShortcutGraph g0( wg );
	ShortcutGraph g1( g0, { g0.fromSource( c ), g0.fromSource( e ), g0.fromSource( k ) } );
	ShortcutGraph g2( g1, { g1.fromSource( b ), g1.fromSource( i ), g1.fromSource( h ), g1.fromSource( l ) } );
	ShortcutGraph g3( g2, { g2.fromSource( a ), g2.fromSource( d ), g2.fromSource( j ) } );

	std::cout << g0 << "\n\n";
	std::cout << g1 << "\n\n";
	std::cout << g2 << "\n\n";
	std::cout << g3 << "\n\n";

	return 0;*/

	for (int i = 1; i < argc; i++)
	{
		std::filesystem::path filepath = argv[i];
		if (filepath.extension() == ".ci")
		{
			CIHierarchy hierarchy;
			std::ifstream file( filepath, std::ios::in | std::ios::binary );
			if (file.is_open())
			{
				deserialize( file, hierarchy );
				hierarchy;
			}
			continue;
		}

		RoadDParser parser( filepath );
		std::optional<WeightedGraph> graph = parser.create();
		if (graph)
		{
			std::cout << std::filesystem::current_path() << "\n";
			CIHierarchy hierarchy( *graph );
			std::filesystem::path savepath = filepath.filename();
			savepath.replace_extension( ".ci" );
			std::ofstream file( savepath, std::ios::out | std::ios::binary );
			serialize( file, hierarchy );
			file.close();
			std::cout << savepath << "\n";
		}
	}

	return 0;
}