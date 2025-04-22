#ifndef HDCALCULATOR_H
#define HDCALCULATOR_H

#include "Util.hpp"

#include "JSON.hpp"

#include "GraphParser.hpp"

class HDCalculator
{
public:
	HDCalculator() = default;
	explicit HDCalculator(Ptr<GraphParser> graphParser);

	void run() const;

private:
	Ptr<GraphParser> _graphParser;
};

JSON_CREATE(HDCalculator)

#endif  // HDCALCULATOR_H