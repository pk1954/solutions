// strlist.c - manage list of strings

#include "stdafx.h"
#include <limits.h>  
#include <string.h>  
#include "negList.h"
#include "errhndl.h"
  
using namespace std;

bool NegativeList::Add
( 
	unsigned long   ulIndex, 
	wstring const & wstr
)
{
	if ( m_negSpecs.end() != m_negSpecs.find( ulIndex ) )
		return false;

	m_negSpecs.emplace( ulIndex, StrInfo( true, wstr ) );   
	return true;
}

bool NegativeList::SetActive
(
	unsigned long ulIndex, 
	bool          fActive 
)
{
	map < unsigned long, StrInfo >::iterator it = m_negSpecs.find( ulIndex );

	if ( m_negSpecs.end() == it )
		return false;
      
	it->second.m_fActive = fActive;
	return true;
}


void NegativeList::SetAllActive( bool fActive )
{
	for ( auto & i: m_negSpecs )
		i.second.m_fActive = fActive;
}

wstring NegativeList::Check( wstring const & wsrtTestling ) const
{
	for ( auto & i: m_negSpecs )
	{
		if ( i.second.m_fActive && ( wstring::npos != wsrtTestling.find( i.second.m_wstrNeg ) ) )
			return i.second.m_wstrNeg;
	}
	
    return L"";  // ok, no negative spec found
}
