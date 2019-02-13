// win32_memorySlot.h
//

#pragma once

#include "EvolutionTypes.h"
#include "win32_shape.h"

class MemorySlot : public Shape
{
public:
	MemorySlot
	( 
		TextDisplay & textDisplay, 
		MEM_INDEX const index 
	) :
		Shape( textDisplay ),
		m_index( index )
	{}

	virtual void      FillBuffer( GridPoint const gp );
	virtual GridPoint GetReferencedGridPoint( GridPoint const ) const; 

	MEM_INDEX GetMemIndex() const {	return m_index;	}

private:
	virtual GridPoint getReferencedGridPoint( IND_ID const ) const;

	IND_ID getIndId( GridPoint const ) const; 

	MEM_INDEX m_index;
};
