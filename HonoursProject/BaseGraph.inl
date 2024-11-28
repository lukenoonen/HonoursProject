#include <utility>
#include <unordered_map>
#include <unordered_set>

template<class VertexType, class EdgeType>
inline BaseGraph<VertexType, EdgeType>::BaseGraph()
	: _numEdges( 0 )
{

}

template <class VertexType, class EdgeType>
BaseGraph<VertexType, EdgeType>::VertexDescriptor BaseGraph<VertexType, EdgeType>::addVertex( VertexType data )
{
	_graph.emplace_back( std::move( data ), EdgeList{} );
	return _graph.size() - 1;
}

template <class VertexType, class EdgeType>
BaseGraph<VertexType, EdgeType>::EdgeDescriptor BaseGraph<VertexType, EdgeType>::addEdge( VertexDescriptor source, VertexDescriptor target, EdgeType data )
{
	_numEdges++;
	_graph[source].second.emplace_back( data, target );
	_graph[target].second.emplace_back( data, source );
	return { source, _graph[source].second.size() - 1 };
}

template <class VertexType, class EdgeType>
void BaseGraph<VertexType, EdgeType>::removeVertices( const std::vector<VertexDescriptor>& remove )
{
	std::unordered_map<VertexDescriptor, VertexDescriptor> removeMap;
	std::unordered_set<VertexDescriptor> removeSet;
	for (const VertexDescriptor v : remove)
	{
		VertexDescriptor v_ = v;
		if (const auto search = removeMap.find( v ); search != removeMap.end())
		{
			v_ = search->second;
		}

		_graph[v_] = _graph.back();
		_graph.pop_back();
		removeMap[_graph.size()] = v_;
		removeSet.insert( v );
	}

	_numEdges = 0;
	for (VertexDescriptor v = 0; v < _graph.size(); v++)
	{
		EdgeList& edgeList = _graph[v].second;
		for (size_t i = 0; i < edgeList.size(); i++)
		{
			EdgeEntry& edgeEntry = edgeList[i];

			bool success = true;
			while (removeSet.contains( edgeEntry.second ))
			{
				if (i == edgeList.size() - 1)
				{
					success = false;
					edgeList.pop_back();
					break;
				}
				else
				{
					edgeEntry = edgeList.back();
					edgeList.pop_back();
				}
			}

			if (!success) { break; }

			if (const auto search = removeMap.find( edgeEntry.second ); search != removeMap.end())
			{
				edgeEntry.second = search->second;
			}

			if (edgeEntry.second >= v)
			{
				_numEdges++;
			}
		}
	}
}

template <class VertexType, class EdgeType>
BaseGraph<VertexType, EdgeType>::VertexDescriptor BaseGraph<VertexType, EdgeType>::source( EdgeDescriptor e ) const
{
	return e.first;
}

template <class VertexType, class EdgeType>
BaseGraph<VertexType, EdgeType>::VertexDescriptor BaseGraph<VertexType, EdgeType>::target( EdgeDescriptor e ) const
{
	return _graph[e.first].second[e.second].second;
}

template <class VertexType, class EdgeType>
BaseGraph<VertexType, EdgeType>::VertexDescriptor BaseGraph<VertexType, EdgeType>::other( EdgeDescriptor e, VertexDescriptor v ) const
{
	const VertexDescriptor s = source( e );
	return s != v ? s : target( e );
}

template <class VertexType, class EdgeType>
VertexType& BaseGraph<VertexType, EdgeType>::get( VertexDescriptor v )
{
	return _graph[v].first;
}

template <class VertexType, class EdgeType>
const VertexType& BaseGraph<VertexType, EdgeType>::get( VertexDescriptor v ) const
{
	return _graph[v].first;
}

template <class VertexType, class EdgeType>
VertexType& BaseGraph<VertexType, EdgeType>::operator[]( VertexDescriptor v )
{
	return get( v );
}

template <class VertexType, class EdgeType>
const VertexType& BaseGraph<VertexType, EdgeType>::operator[]( VertexDescriptor v ) const
{
	return get( v );
}

template <class VertexType, class EdgeType>
EdgeType& BaseGraph<VertexType, EdgeType>::get( EdgeDescriptor e )
{
	return _graph[e.first].second[e.second].first;
}

template <class VertexType, class EdgeType>
const EdgeType& BaseGraph<VertexType, EdgeType>::get( EdgeDescriptor e ) const
{
	return _graph[e.first].second[e.second].first;
}

template <class VertexType, class EdgeType>
EdgeType& BaseGraph<VertexType, EdgeType>::operator[]( EdgeDescriptor e )
{
	return get( e );
}

template <class VertexType, class EdgeType>
const EdgeType& BaseGraph<VertexType, EdgeType>::operator[]( EdgeDescriptor e ) const
{
	return get( e );
}

template <class VertexType, class EdgeType>
size_t BaseGraph<VertexType, EdgeType>::numVertices() const
{
	return _graph.size();
}

template <class VertexType, class EdgeType>
size_t BaseGraph<VertexType, EdgeType>::numEdges() const
{
	return _numEdges;
}

template <class VertexType, class EdgeType>
size_t BaseGraph<VertexType, EdgeType>::degree( VertexDescriptor v ) const
{
	return _graph[v].size();
}


template <class VertexType, class EdgeType>
template <class P>
void BaseGraph<VertexType, EdgeType>::vertexMap( P predicate ) const
{
	for (VertexDescriptor v = 0; v < _graph.size(); v++)
	{
		if (predicate( v )) { break; }
	}
}

template <class VertexType, class EdgeType>
template <class P>
void BaseGraph<VertexType, EdgeType>::vertexMap( VertexDescriptor v, P predicate ) const
{
	const EdgeList& edgeList = _graph[v].second;
	for (const EdgeEntry& edgeEntry : edgeList)
	{
		if (predicate( edgeEntry.second )) { break; }
	}
}

template <class VertexType, class EdgeType>
template <class P>
void BaseGraph<VertexType, EdgeType>::edgeMap( P predicate ) const
{
	for (VertexDescriptor v = 0; v < _graph.size(); v++)
	{
		const EdgeList& edgeList = _graph[v].second;
		for (size_t i = 0; i < edgeList.size(); i++)
		{
			if (edgeList[i].second >= v) { continue; }
			if (predicate( { v, i } )) { break; }
		}
	}
}

template <class VertexType, class EdgeType>
template <class P>
void BaseGraph<VertexType, EdgeType>::edgeMap( VertexDescriptor v, P predicate ) const
{
	const EdgeList& edgeList = _graph[v].second;
	for (size_t i = 0; i < edgeList.size(); i++)
	{
		if (predicate( { v, i } )) { break; }
	}
}

template<class VertexType, class EdgeType>
inline std::ostream& operator<<( std::ostream& os, const BaseGraph<VertexType, EdgeType>& g )
{
	using VertexDescriptor = BaseGraph<VertexType, EdgeType>::VertexDescriptor;
	using GraphType        = BaseGraph<VertexType, EdgeType>::GraphType;

	const GraphType& graph = g._graph;

	std::cout << g.numVertices() << ", " << g.numEdges() << '\n';

	for (VertexDescriptor v = 0; v < graph.size(); v++)
	{
		os << v << " ->";
		const BaseGraph<VertexType, EdgeType>::EdgeList& edgeList = graph[v].second;
		if (edgeList.empty())
		{
			os << " n/a";
		}
		else
		{
			for (size_t i = 0; i < edgeList.size(); i++)
			{
				os << ' ' << edgeList[i].second;
			}
		}
		if (v != graph.size() - 1) { os << '\n'; }
	}

	return os;
}