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

	return 0;
}