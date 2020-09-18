// PipeList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Pipe.h"

using std::vector;

using PipeFunc = function<void(Pipe &)>;
using PipeCrit = function<bool(Pipe &)>;

class PipeList
{
public:

size_t  const   Size()     const { return   m_list.size(); }
bool    const   IsEmpty()  const { return   m_list.empty(); }

Pipe       & GetFirst     ()       { return * m_list.at( 0 ); }
Pipe const & GetFirstConst() const { return * m_list.at( 0 ); }

void ClearPipeList( ) {	m_list.clear(); }

void AddPipe( Pipe * const pPipe )
{
	if ( pPipe != nullptr )
	{
		assert( find( begin(m_list), end(m_list), pPipe ) == end(m_list) );
		m_list.push_back( pPipe );
	}
}

void RemovePipe( Pipe * const pPipe )
{
	auto res = find( begin(m_list), end(m_list), pPipe );
	assert( res != end(m_list) );
	m_list.erase( res );
}

void ReplacePipe( Pipe * const pDel, Pipe * const pAdd )
{
	replace( begin(m_list), end(m_list), pDel, pAdd );
}

void Apply2AllPipesInList( PipeFunc const & func ) const
{
	for ( auto pPipe : m_list ) 
	{ 
		if ( pPipe != nullptr )
			func( * pPipe );
	}
}

bool Apply2AllPipesInListB( PipeCrit const & func ) const 
{
	bool bResult { false };
	for ( auto pPipe : m_list ) 
	{ 
		if ( pPipe != nullptr )
		{
			if ( func( * pPipe ) )
				return true;
		}
	}
	return false;
}

private:
	vector<Pipe *> m_list;
};