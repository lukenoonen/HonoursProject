#ifndef HDCALCULATOR_H
#define HDCALCULATOR_H

#include "Util.h"

#include "JSON.h"

#include "GraphParser.h"

class HDCalculator
{
public:
	HDCalculator() = default;
	HDCalculator( Ptr<GraphParser> graphParser );

	void run() const;

private:
	Ptr<GraphParser> _graphParser;
};

JSON_CREATE( HDCalculator )

#endif // HDCALCULATOR_H