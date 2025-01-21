#include "TestSuite.h"

#include <iostream>

int main( int argc, const char* argv[] )
{
	if (argc < 2)
	{
		std::cerr << "No test suite provided\n";
		return -1;
	}

	FilePath filepath = argv[1];
	std::ifstream file( std::move( filepath ), std::ios::in );
	if (!file.is_open()) { return -1; }

	try
	{
		const JSON json = JSON::parse( file );
		g_logger = std::move( json["logger"].get<Logger>() );
		TestSuite tests = json["suite"].get<TestSuite>();
		tests.run();
	}
	catch (JSON::exception e)
	{
		std::cout << e.what() << "\n";
	}

	/*
	for (int i = 1; i < argc; i++)
	{
		FilePath filepath = argv[i];
		if (filepath.extension() == ".ci")
		{
			CIShortcutHierarchy hierarchy;
			std::ifstream file( filepath, std::ios::in | std::ios::binary );
			if (file.is_open())
			{
				deserialize( file, hierarchy );
				hierarchy;
			}
			continue;
		}

		RoadDParser parser( filepath );
		Opt<WeightedGraph> graph = parser.create();
		if (graph)
		{
			std::cout << std::filesystem::current_path() << "\n";
			CIShortcutHierarchy hierarchy( *graph );
			FilePath savepath = filepath.filename();
			savepath.replace_extension( ".ci" );
			std::ofstream file( savepath, std::ios::out | std::ios::binary );
			serialize( file, hierarchy );
			file.close();
			std::cout << savepath << "\n";
		}
	}*/

	return 0;
}