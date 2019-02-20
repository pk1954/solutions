// win32_rightColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include "win32_memorySlot.h"
#include "win32_rightColumn.h"

using std::wostringstream;

RightColumn::RightColumn( TextDisplay & textDisplay ) :
	Shape( textDisplay )
{
	for	( MEM_INDEX mem = MEM_INDEX( 0 ); mem < MEM_INDEX(IMEMSIZE_MAX); ++mem )
	{
		m_aMemorySlot[mem.GetValue()] = new MemorySlot( textDisplay, mem );
	}
}

PixelRectSize RightColumn::MinimalSize( )  
{       
	PixelRectSize minSlotSize = m_aMemorySlot[0]->MinimalSize( );
	return SetMinSize( minSlotSize.GetX( ), minSlotSize.GetY( ) * IMEMSIZE_MAX	);     
}                                     

void RightColumn::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( SetShapeRect( ppOffset, ppSize ) )
	{
		PIXEL_Y       const pixSlotHeight{ getShapeHeight() / IMEMSIZE_MAX };
		PixelRectSize const slotSize     { getShapeWidth(), pixSlotHeight };

		PixelPoint posShape = GetShapePos( );
		for	( auto & pSlot : m_aMemorySlot )
		{
			posShape += PixelPoint( PIXEL_X(0_PIXEL), pixSlotHeight );
			pSlot->PrepareShape( posShape, slotSize );
		}
	}
}

void RightColumn::FillBuffer( GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();
	EvolutionCore  & core   = m_textDisplay.Core();

	MEM_INDEX const memSize = core.GetMemSize( gp );  
	MEM_INDEX const memUsed = core.GetMemUsed( gp ); 
        
	buffer << L"Memory " << memUsed << L"/" << memSize;
}

Shape const * RightColumn::FindShape
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
		if ( pSlot->FindShape( pnt, gp ) )
			return pSlot;
	}

	return Shape::FindShape( pnt, gp );
}

void RightColumn::Draw( GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( IsNotEmpty () )
	{
		Shape::Draw( gp, ppGridpointOffset );

		MEM_INDEX const memUsed = m_textDisplay.Core().GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			pSlot->Draw( gp, ppGridpointOffset );
		}
	}
}
