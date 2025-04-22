#ifndef ANALYSER_H
#define ANALYSER_H

#include "Util.hpp"

#include "JSON.hpp"

#include "Factory.hpp"

class Analyser
{
public:
	Analyser()                           = default;
	virtual ~Analyser()                  = default;
	Analyser(const Analyser&)            = default;
	Analyser& operator=(const Analyser&) = default;
	Analyser(Analyser&&)                 = default;
	Analyser& operator=(Analyser&&)      = default;

	virtual void analyse() const = 0;
};

FACTORY_CREATE_BASE_JSON(Analyser)

JSON_CREATE_FACTORY(Analyser)

#endif  // ANALYSER_H