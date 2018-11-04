// win32_rightColumn.cpp
//

#pragma once

#include <array>
#include <iomanip>
#include "EvolutionTypes.h"
#include "gridRect.h"
#include "win32_memorySlot.h"
#include "win32_gridPointShape.h"

using namespace std;

class RightColumn : public GridPointShape
{
public:
	RightColumn
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		GridPointShape( pParent, textDisplay )
	{
		for	( MEM_INDEX mem = 0; mem < IMEMSIZE_MAX; ++mem )
		{
			m_aMemorySlot[mem] = new MemorySlot( this, textDisplay, mem );
		}
	}

	virtual void PrepareShape( GridPoint const gp )
	{
		PixelRectSize   rectSize = GetSize();
		long      const lHeight  = rectSize.GetHeight() / (IMEMSIZE_MAX + 1);
		long            lYpos    = rectSize.GetHeight() - lHeight;

		rectSize.SetHeight( lHeight );

		MEM_INDEX const memUsed = m_textDisplay.Core().GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			lYpos -= lHeight;
			pSlot->SetSize( rectSize );
			pSlot->SetShapeOffset( PixelPoint( 0, lYpos ) );
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

	GridPointShape const * FindShape
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
		GridPointShape::Draw( gp );

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
