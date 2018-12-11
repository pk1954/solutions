// win32_memorySlot.h
//

#pragma once

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

	virtual GridPoint GetReferencedGridPoint( GridPoint const ) const; 

	virtual void FillBuffer( GridPoint const gp );

	MEM_INDEX GetMemIndex() const
	{
		return m_index;
	}

private:
	virtual GridPoint getReferencedGridPoint( IndId const ) const;

	IndId getIndId( GridPoint const ) const; 

	MEM_INDEX m_index;
};
