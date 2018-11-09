// win32_rightColumn.cpp
//

#pragma once

#include <array>
#include <iomanip>
#include "EvolutionTypes.h"
#include "gridRect.h"
#include "win32_memorySlot.h"
#include "win32_shape.h"

using namespace std;

class RightColumn : public Shape
{
public:
	RightColumn
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		Shape( pParent, textDisplay )
	{
		for	( MEM_INDEX mem = 0; mem < IMEMSIZE_MAX; ++mem )
		{
			m_aMemorySlot[mem] = new MemorySlot( this, textDisplay, mem );
		}
	}

	virtual void PrepareShape( GridPoint const gp )
	{
		long          const lShapeHeight = GetShapeHeight();
		long          const lHeight      = lShapeHeight / (IMEMSIZE_MAX + 1);
		PixelRectSize const slotSize( GetShapeWidth(), lHeight );

		long lYpos = lHeight;
		MEM_INDEX const memUsed = m_textDisplay.Core().GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			lYpos += lHeight;
			pSlot->SetShapeRect( PixelRect( PixelPoint( 0, lYpos ), slotSize ) );
		}
	}

	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		EvolutionCore  & core   = m_textDisplay.Core();

		MEM_INDEX const memSize = core.GetMemSize( gp );  
		MEM_INDEX const memUsed = core.GetMemUsed( gp ); 
        
		buffer << L"  Mem " << memUsed << L"/" << memSize;
	}

	Shape const * FindShape
	( 
		PixelPoint const pnt,             
		GridPoint  const gp
	) const
	{
		MEM_INDEX const memUsed = m_textDisplay.Core().GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			if ( pSlot->PointInShape(pnt) )
				return pSlot;
		}

		return nullptr;
	}

	void Draw( GridPoint const gp )
	{
		Shape::Draw( gp );

		MEM_INDEX const memUsed = m_textDisplay.Core().GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			pSlot->Draw( gp );
		}
	}

private:
    array< MemorySlot *, IMEMSIZE_MAX > m_aMemorySlot; 
};
