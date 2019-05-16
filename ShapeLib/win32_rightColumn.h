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
	RightColumn( );

	virtual void          SetTextDisplay( TextDisplay * );
	virtual void          PrepareShape( PixelPoint const, PixelRectSize const );
	virtual PixelRectSize MinimalSize ( EvolutionCore const * const );
	virtual void          Draw        ( EvolutionCore const * const, GridPoint const, PixelPoint const  );
	virtual void          FillBuffer  ( EvolutionCore const * const, GridPoint const );

	virtual Shape const * FindShape( EvolutionCore const * const, PixelPoint const, GridPoint const ) const;

private:
    std::array< MemorySlot *, IMEMSIZE_MAX > m_aMemorySlot; 
};
