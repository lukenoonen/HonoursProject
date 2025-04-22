#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "Util.hpp"

#include "JSON.hpp"

#include "Analyser.hpp"

class Analysis
{
public:
	Analysis() = default;
	explicit Analysis(Vec<Ptr<Analyser>> analysers);

	void analyse() const;

private:
	Vec<Ptr<Analyser>> _analysers;
};

JSON_CREATE(Analysis)

#endif  // ANALYSIS_H