// win32_rightColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include "win32_memorySlot.h"
#include "win32_rightColumn.h"

using namespace std;

RightColumn::RightColumn( TextDisplay & textDisplay ) :
	Shape( textDisplay )
{
	for	( MEM_INDEX mem = 0; mem < IMEMSIZE_MAX; ++mem )
	{
		m_aMemorySlot[mem] = new MemorySlot( textDisplay, mem );
	}
}

PixelRectSize RightColumn::MinimalSize( )  
{       
	for	( auto & pSlot : m_aMemorySlot )
	{
		pSlot->MinimalSize( );
	}

	return setMinSize
	( 
		2 * MARGIN + m_aMemorySlot[0]->GetMinWidth ( ), 
		2 * MARGIN + m_aMemorySlot[0]->GetMinHeight( ) * IMEMSIZE_MAX
	);     
}                                     

void RightColumn::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( setShapeRect( ppOffset, ppSize ) )
	{
		long          const slotHeight{ getShapeHeight() / (IMEMSIZE_MAX + 1) };
		PixelRectSize const slotSize  { getShapeWidth(), slotHeight };

		PixelPoint pixPosSubShape = getShapePos( );
		for	( auto & pSlot : m_aMemorySlot )
		{
			pixPosSubShape.y += slotHeight;
			pSlot->PrepareShape( pixPosSubShape, slotSize );
		}
	}
}

void RightColumn::FillBuffer( GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();
	EvolutionCore  & core   = m_textDisplay.Core();

	MEM_INDEX const memSize = core.GetMemSize( gp );  
	MEM_INDEX const memUsed = core.GetMemUsed( gp ); 
        
	buffer << L"Mem " << memUsed << L"/" << memSize;
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
	if ( isNotEmpty () )
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
