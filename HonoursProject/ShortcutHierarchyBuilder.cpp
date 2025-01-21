#include "ShortcutHierarchyBuilder.h"

ShortcutHierarchyBuilder::ShortcutHierarchyBuilder( FilePath filepath, const WeightedGraph& graph )
	: PathSolverBuilder( graph ),
	  _filepath( std::move( filepath ) )
{

}

Ptr<PathSolver> ShortcutHierarchyBuilder::createInternal() const
{
	auto result = load();
	if (result) { return result; }
	return build();
}

Ptr<PathSolver> ShortcutHierarchyBuilder::load() const
{
	Ptr<ShortcutHierarchy> result = std::make_unique<ShortcutHierarchy>();
	if (!_filepath.empty() && deserialize(_filepath, *result)) { return result; }
	return nullptr;
}

Ptr<PathSolver> ShortcutHierarchyBuilder::build() const
{
	Ptr<ShortcutHierarchy> result = buildInternal();
	if (!_filepath.empty()) { serialize( _filepath, *result ); }
	return result;
}
