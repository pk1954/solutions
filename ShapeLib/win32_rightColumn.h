// win32_rightColumn.cpp
//

#pragma once

#include <array>
#include "EvolutionTypes.h"
#include "win32_shape.h"

class MemorySlot;

class RightColumn : public Shape
{
public:
	RightColumn( TextDisplay & );

	virtual PixelRectSize MinimalSize( );
	virtual void          PrepareShape  ( PixelPoint const, PixelRectSize const );
	virtual void          Draw          ( GridPoint const, PixelPoint const  );
	virtual void          FillBuffer    ( GridPoint const );

	virtual Shape const * FindShape( PixelPoint const, GridPoint const ) const;

private:
    array< MemorySlot *, IMEMSIZE_MAX > m_aMemorySlot; 
};
