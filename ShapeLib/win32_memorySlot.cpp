// win32_memorySlot.cpp
//

#include "stdafx.h"
#include <iomanip>
#include "win32_memorySlot.h"

using std::wostringstream;

GridPoint MemorySlot::GetReferencedGridPoint( EvolutionCore const * const pCore, GridPoint const gp ) const 
{ 
	return getReferencedGridPoint( pCore, getIndId( pCore, gp ) );
}

void MemorySlot::FillBuffer( EvolutionCore const * const pCore, GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();
	IND_ID           indId  = getIndId( pCore, gp );

	buffer << std::setw( 10 );
	if ( getReferencedGridPoint( pCore, indId ) == GP_NULL ) 
		buffer << L"DEAD";
	else
		buffer << std::setbase( 16 ) << indId << std::setbase( 10 );
}

GridPoint MemorySlot::getReferencedGridPoint( EvolutionCore const * const pCore, IND_ID const indId ) const 
{ 
	return pCore->FindGridPoint( indId );
}

IND_ID MemorySlot::getIndId( EvolutionCore const * const pCore, GridPoint const gp ) const 
{ 
	return pCore->GetMemEntry( gp, m_index );
}
