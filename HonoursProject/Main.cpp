/*#include "Analysis.hpp"
#include "HDCalculator.hpp"
#include "TestSuite.hpp"
#include "Interactor.hpp"

namespace
{
	enum class OptionType : unsigned char
	{
		SHORT,
		LONG,

		COUNT,
		INVALID = COUNT
	};

	OptionType extractOptionType(const char* arg)
	{
		if (arg[0] == '-')
		{
			if (arg[1] == '-') { return OptionType::LONG; }
			
			return OptionType::SHORT;
		}
		return OptionType::INVALID;
	}

	enum class Option : unsigned char
	{
		LOGGER,
		TEST,
		ANALYSE,
		INTERACT,
		CALC_HD,

		COUNT,
		INVALID = COUNT
	};

	Option extractOption(const char* arg)
	{
		const OptionType type = extractOptionType(arg);
		switch (type)
		{
			case OptionType::SHORT:
			{
				const Str cmp(arg + 1);
				if (cmp == "l") { return Option::LOGGER; }
				if (cmp == "t") { return Option::TEST; }
				if (cmp == "a") { return Option::ANALYSE; }
				if (cmp == "i") { return Option::INTERACT; }
				if (cmp == "h") { return Option::CALC_HD; }
				break;
			}
			case OptionType::LONG:
			{
				const Str cmp(arg + 2);
				if (cmp == "logger") { return Option::LOGGER; }
				if (cmp == "test") { return Option::TEST; }
				if (cmp == "analyse") { return Option::ANALYSE; }
				if (cmp == "interact") { return Option::INTERACT; }
				if (cmp == "calc_hd") { return Option::CALC_HD; }
				break;
			}
			default:
			{
				break;
			}
		}
		return Option::INVALID;
	}

	Opt<std::ifstream> openFile(const FilePath& filepath)
	{
		std::ifstream file(filepath, std::ios::in);
		if (!file.is_open())
		{
			g_logger.log("Failed to open file {}\n", filepath.string());
			return {};
		}

		return file;
	}

	void logger(const char* arg)
	{
		auto file = openFile(arg);
		if (!file) { return; }

		try
		{
			const JSON json = JSON::parse(*file);
			g_logger        = std::move(json.get<Logger>());
		}
		catch (const JSON::exception& e)
		{
			g_logger.log("Failed to parse logger file {}: {}\n", arg, e.what());
		}

		file->close();
	}

	void test(const char* arg)
	{
		auto file = openFile(arg);
		if (!file) { return; }

		try
		{
			const JSON      json  = JSON::parse(*file);
			const TestSuite tests = json.get<TestSuite>();
			tests.run();
		}
		catch (const JSON::exception& e)
		{
			g_logger.log("Failed to parse test file {}: {}\n", arg, e.what());
		}

		file->close();
	}

	void analyse(const char* arg)
	{
		auto file = openFile(arg);
		if (!file) { return; }

		try
		{
			const JSON     json     = JSON::parse(*file);
			const Analysis analysis = json.get<Analysis>();
			analysis.analyse();
		}
		catch (const JSON::exception& e)
		{
			g_logger.log("Failed to parse analyser file {}: {}\n", arg, e.what());
		}

		file->close();
	}

	void interact(const char* arg)
	{
		auto file = openFile(arg);
		if (!file) { return; }

		try
		{
			const JSON       json = JSON::parse(*file);
			Interactor interactor = json.get<Interactor>();
			interactor.start();
		}
		catch (const JSON::exception& e)
		{
			g_logger.log("Failed to parse interactor file {}: {}\n", arg, e.what());
		}

		file->close();
	}

	void calcHD(const char* arg)
	{
		auto file = openFile(arg);
		if (!file) { return; }

		try
		{
			const JSON         json         = JSON::parse(*file);
			const HDCalculator hdCalculator = json.get<HDCalculator>();
			hdCalculator.run();
		}
		catch (const JSON::exception& e)
		{
			g_logger.log("Failed to parse hd file {}: {}\n", arg, e.what());
		}

		file->close();
	}
}  // namespace

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		g_logger.log("No options provided\n");
		return -1;
	}

	for (int i = 1; i < argc; i++)
	{
		const char*  arg    = argv[i];
		const Option option = extractOption(argv[i]);

		switch (option)
		{
			case Option::LOGGER:
			case Option::TEST:
			case Option::ANALYSE:
			case Option::INTERACT:
			case Option::CALC_HD:
			{
				i++;
				if (i >= argc)
				{
					g_logger.log(
						"No parameter provided for {}th argument {}\n", i, arg
					);
					continue;
				}
				break;
			}
			default:
			{
				break;
			}
		}

		switch (option)
		{
			case Option::LOGGER:
			{
				logger(argv[i]);
				break;
			}
			case Option::TEST:
			{
				test(argv[i]);
				break;
			}
			case Option::ANALYSE:
			{
				analyse(argv[i]);
				break;
			}
			case Option::INTERACT:
			{
				interact(argv[i]);
				break;
			}
			case Option::CALC_HD:
			{
				calcHD(argv[i]);
				break;
			}
			default:
			{
				g_logger.log("Failed to parse option {}\n", arg);
				break;
			}
		}
	}

	return 0;
}*/

#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	QPushButton button1 ("test");
	QPushButton button2 ("other", &button1);

	button1.show();

	return app.exec();
}