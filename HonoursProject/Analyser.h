#ifndef ANALYSER_H
#define ANALYSER_H

#include "Util.h"

#include "JSON.h"

#include "Factory.h"

class Analyser
{
public:
	virtual ~Analyser() = default;

	virtual void analyse() const = 0;
};

FACTORY_CREATE_BASE_JSON( Analyser )

JSON_CREATE_FACTORY( Analyser )

#endif // ANALYSER_H