// win32_memorySlot.h
//

#pragma once

#include "EvolutionTypes.h"
#include "win32_shape.h"

class MemorySlot : public Shape
{
public:
	MemorySlot( MEM_INDEX const index ) :
		m_index( index )
	{}

	virtual void      FillBuffer            ( EvolutionCore const * const, GridPoint const );
	virtual GridPoint GetReferencedGridPoint( EvolutionCore const * const, GridPoint const ) const; 

	MEM_INDEX GetMemIndex() const {	return m_index;	}

private:
	virtual GridPoint getReferencedGridPoint( EvolutionCore const * const, IND_ID const ) const;

	IND_ID getIndId( EvolutionCore const * const, GridPoint const ) const; 

	MEM_INDEX m_index;
};
