// win32_rightColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include "win32_memorySlot.h"
#include "win32_rightColumn.h"

using std::wostringstream;

RightColumn::RightColumn( TextDisplay & textDisplay ) :
	Shape( textDisplay ),
	m_aMemorySlot()
{
	for	( MEM_INDEX mem = MEM_INDEX( 0 ); mem < MEM_INDEX(IMEMSIZE_MAX); ++mem )
	{
		m_aMemorySlot[mem.GetValue()] = new MemorySlot( textDisplay, mem );
	}
}

PixelRectSize RightColumn::MinimalSize( EvolutionCore const * const pCore )  
{       
	PixelRectSize minSlotSize = m_aMemorySlot[0]->MinimalSize( pCore );
	return SetMinSize( minSlotSize.GetX( ), minSlotSize.GetY( ) * IMEMSIZE_MAX	);     
}                                     

void RightColumn::PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
{
	if ( SetShapeRect( ppOffset, ppSize ) )
	{
		PIXEL         const pixSlotHeight{ getShapeHeight() / IMEMSIZE_MAX };
		PixelRectSize const slotSize     { getShapeWidth(), pixSlotHeight };

		PixelPoint posShape = GetShapePos( );
		for	( auto & pSlot : m_aMemorySlot )
		{
			posShape += PixelPoint( 0_PIXEL, pixSlotHeight );
			pSlot->PrepareShape( posShape, slotSize );
		}
	}
}

void RightColumn::FillBuffer( EvolutionCore const * const pCore, GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();

	MEM_INDEX const memSize = pCore->GetMemSize( gp );  
	MEM_INDEX const memUsed = pCore->GetMemUsed( gp ); 
        
	buffer << L"Memory " << memUsed.GetValue() << L"/" << memSize.GetValue();
}

Shape const * RightColumn::FindShape
( 
	EvolutionCore const * const pCore, 
	PixelPoint            const pnt,             
	GridPoint             const gp
) const
{
	MEM_INDEX const memUsed = pCore->GetMemUsed( gp ); 
	for	( auto & pSlot : m_aMemorySlot )
	{
		if ( pSlot->GetMemIndex() == memUsed )
			break;
		if ( pSlot->FindShape( pnt, gp ) )
			return pSlot;
	}

	return Shape::FindShape( pnt, gp );
}

void RightColumn::Draw
( 
	EvolutionCore const * const pCore, 
	GridPoint             const gp, 
	PixelPoint            const pntGridpointOffset 
)
{
	if ( IsNotEmpty () )
	{
		Shape::Draw( pCore, gp, pntGridpointOffset );

		MEM_INDEX const memUsed = pCore->GetMemUsed( gp ); 
		for	( auto & pSlot : m_aMemorySlot )
		{
			if ( pSlot->GetMemIndex() == memUsed )
				break;
			pSlot->Draw( pCore, gp, pntGridpointOffset );
		}
	}
}
