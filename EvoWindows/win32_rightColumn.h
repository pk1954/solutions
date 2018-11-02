// win32_rightColumn.cpp
//

#pragma once

#include <array>
#include <iomanip>
#include "EvolutionTypes.h"
#include "gridRect.h"
#include "win32_gridPointShape.h"

using namespace std;

class MemorySlot : public GridPointShape
{
public:
	MemorySlot
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay, 
		MEM_INDEX     const index 
	) :
		GridPointShape( pParent, textDisplay ),
		m_index( index )
	{}

	virtual void PrepareShape( )
	{ }

	virtual GridPoint GetReferencedGridPoint( GridPoint const gp ) const 
	{ 
		return getReferencedGridPoint( getIndId( gp ) );
	}

	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		IndId            indId  = getIndId( gp );

		buffer << setw( 10 );
		if (getReferencedGridPoint( indId ) == GridPoint::GP_NULL) 
			buffer << L"DEAD";
		else
			buffer << indId;
	}

	MEM_INDEX GetMemIndex()
	{
		return m_index;
	}

private:
	virtual GridPoint getReferencedGridPoint( IndId const indId ) const 
	{ 
		return m_textDisplay.Core().FindGridPoint( indId );
	}

	IndId getIndId( GridPoint const gp ) const 
	{ 
		return m_textDisplay.Core().GetMemEntry( gp, m_index );
	}

	MEM_INDEX m_index;
};

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

	virtual void PrepareShape( )
	{
		PixelRectSize   rectSize = GetSize();
		long      const lHeight  = rectSize.GetHeight() / (IMEMSIZE_MAX + 1);
		long            lYpos    = rectSize.GetHeight() - lHeight;

		rectSize.SetHeight( lHeight );

		for	( auto & pSlot : m_aMemorySlot )
		{
			lYpos -= lHeight;
			pSlot->SetSize( rectSize );
			pSlot->SetShapeOffset( PixelPoint( 0, lYpos ) );
		}
	}

	GridPointShape const * FindShape
	( 
		PixelPoint const & pnt,             
		GridPoint  const   gp
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

	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		EvolutionCore  & core   = m_textDisplay.Core();

		MEM_INDEX const memSize = core.GetMemSize( gp );  
		MEM_INDEX const memUsed = core.GetMemUsed( gp ); 
        
		buffer << L"  Mem " << memUsed << L"/" << memSize;
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
