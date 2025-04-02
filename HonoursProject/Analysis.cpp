#include "Analysis.h"

Analysis::Analysis( Vec<Ptr<Analyser>> analysers )
	: _analysers( std::move( analysers ) )
{

}

void Analysis::analyse() const
{
	for (const auto& analyser : _analysers)
	{
		analyser->analyse();
	}
}

JSON_BEGIN( Analysis )

	JSON_ARG( Vec<Ptr<Analyser>>, analysers )

	JSON_FABRICATE( std::move( analysers ) )

JSON_END()