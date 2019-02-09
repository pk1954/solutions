// win32_memorySlot.cpp
//

#include "stdafx.h"
#include <iomanip>
#include "win32_memorySlot.h"

using std::wostringstream;

GridPoint MemorySlot::GetReferencedGridPoint( GridPoint const gp ) const 
{ 
	return getReferencedGridPoint( getIndId( gp ) );
}

void MemorySlot::FillBuffer( GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();
	IndividualId     indId  = getIndId( gp );

	buffer << std::setw( 10 );
	if ( getReferencedGridPoint( indId ) == GridPoint::NULL_VAL() ) 
		buffer << L"DEAD";
	else
		buffer << indId;
}

GridPoint MemorySlot::getReferencedGridPoint( IndividualId const indId ) const 
{ 
	return m_textDisplay.Core().FindGridPoint( indId );
}

IndividualId MemorySlot::getIndId( GridPoint const gp ) const 
{ 
	return m_textDisplay.Core().GetMemEntry( gp, m_index );
}
