#include "BuildTimesAnalyser.h"
#include "Profiler.h"
#include "JSON.h"
#include "Serialize.h"

BuildTimesAnalyser::BuildTimesAnalyser( Vec<Entry> entries, FilePath output )
	: _entries( std::move( entries ) ),
	  _output( std::move( output ) )
{

}

void BuildTimesAnalyser::analyse() const
{
	Vec<JSON> entryJSONs;
	entryJSONs.reserve( _entries.size() );
	for (const Entry& entry : _entries)
	{
		Vec<JSON> fileJSONs;
		fileJSONs.reserve( entry.files().size() );
		for (const Entry::File& file : entry.files())
		{
			ProfilerSet::Times buildtimes;
			deserialize( file.filepath(), buildtimes );
			JSON fileJSON = { { "type", file.type() } };
			for (const auto& [key, bt] : buildtimes)
			{
				fileJSON[key] = bt.first * 0.001;
			}
			fileJSONs.push_back( std::move( fileJSON ) );
		}
		entryJSONs.push_back( {
			{ "name", entry.name() },
			{ "pathSolvers", std::move( fileJSONs ) }
		} );
	}

	const JSON dataJSON = { { "buildtimes", std::move( entryJSONs ) } };

	std::ofstream out( _output );
	out << std::setw( 4 ) << dataJSON;
}

FACTORY_BEGIN_JSON( "buildtimes", BuildTimesAnalyser, Analyser )

	JSON_ARG( Vec<BuildTimesAnalyser::Entry>, entries )
	JSON_ARG( Str, output )

	FACTORY_FABRICATE( std::move( entries ), std::move( output ) )

FACTORY_END()

BuildTimesAnalyser::Entry::Entry( Str name, Vec<File> files )
	: _name( std::move( name ) ),
	  _files( std::move( files ) )
{

}

const Str& BuildTimesAnalyser::Entry::name() const
{
	return _name;
}

const Vec<BuildTimesAnalyser::Entry::File>& BuildTimesAnalyser::Entry::files() const
{
	return _files;
}

JSON_BEGIN( BuildTimesAnalyser::Entry )

	JSON_ARG( Str, name )
	JSON_ARG( Vec<BuildTimesAnalyser::Entry::File>, files )

	JSON_FABRICATE( std::move( name ), std::move( files ) )

JSON_END()

BuildTimesAnalyser::Entry::File::File( Str type, FilePath filepath )
	: _type( std::move( type ) ),
	  _filepath( std::move( filepath ) )
{

}

const Str& BuildTimesAnalyser::Entry::File::type() const
{
	return _type;
}

const FilePath& BuildTimesAnalyser::Entry::File::filepath() const
{
	return _filepath;
}

JSON_BEGIN( BuildTimesAnalyser::Entry::File )

	JSON_ARG( Str, type )
	JSON_ARG( Str, filepath )

	JSON_FABRICATE( std::move( type ), std::move( filepath ) )

JSON_END()