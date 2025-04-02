#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "Util.h"

#include "JSON.h"

#include "Analyser.h"

class Analysis
{
public:
	Analysis() = default;
	Analysis( Vec<Ptr<Analyser>> analysers );

	void analyse() const;

private:
	Vec<Ptr<Analyser>> _analysers;
};

JSON_CREATE( Analysis )

#endif // ANALYSIS_H