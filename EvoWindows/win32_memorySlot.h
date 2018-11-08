// win32_memorySlot.cpp
//

#pragma once

#include <iomanip>
#include "win32_shape.h"

using namespace std;

class MemorySlot : public Shape
{
public:
	MemorySlot
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay, 
		MEM_INDEX     const index 
	) :
		Shape( pParent, textDisplay ),
		m_index( index )
	{}

	virtual GridPoint GetReferencedGridPoint( GridPoint const gp ) const 
	{ 
		return getReferencedGridPoint( getIndId( gp ) );
	}

	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		IndId            indId  = getIndId( gp );

		buffer << setw( 10 );
		if (getReferencedGridPoint( indId ) == GridPoint::GP_NULL) 
			buffer << L"DEAD";
		else
			buffer << indId;
	}

	MEM_INDEX GetMemIndex()
	{
		return m_index;
	}

private:
	virtual GridPoint getReferencedGridPoint( IndId const indId ) const 
	{ 
		return m_textDisplay.Core().FindGridPoint( indId );
	}

	IndId getIndId( GridPoint const gp ) const 
	{ 
		return m_textDisplay.Core().GetMemEntry( gp, m_index );
	}

	MEM_INDEX m_index;
};
