// win32_shape.h : 
//
// Shape is defined by the offset of the top left corner
// and the size of the rectangle 

#pragma once

#include "windows.h"
#include "GridDimensions.h"
#include "win32_textDisplay.h"
#include "PixelTypes.h"

class Shape
{
public:
	Shape( TextDisplay & t ) :
		m_textDisplay( t ),
		m_rect   ( PixelRect( ) ),
		m_minSize( PixelRectSize( 0_PIXEL ) )
	{}

	virtual PixelRectSize MinimalSize( EvolutionCore const * const pCore )  
	{                                     
		m_textDisplay.Clear();
		FillBuffer( pCore, GridPoint::ZERO_VAL() );
		return SetMinSize( m_textDisplay.CalcRectSize( ) );
	}                                     

	PixelRect const GetAbsoluteShapeRect( GridPoint const gp ) const 
	{
		return m_rect + m_textDisplay.GetOffset( gp );
	}

	PIXEL const GetMinWidth ( ) const { return m_minSize.GetX();  }
	PIXEL const GetMinHeight( ) const { return m_minSize.GetY(); }

	virtual Shape const * FindShape  // for all shapes without subshapes
	( 
		PixelPoint const pnt,             
		GridPoint  const gp
	) const
	{
		return m_rect.Includes( pnt ) ? this : nullptr;
	}

	virtual GridPoint GetReferencedGridPoint( EvolutionCore const * const pCore, GridPoint const gp ) const 
	{ 
		return GridPoint::NULL_VAL(); 
	}

	virtual void Draw( EvolutionCore const * const, GridPoint const, PixelPoint const );
	virtual void AddContextMenuEntries( HMENU const ) const {};

	// PrepareShape: Rearrange shape according to new size
	// no GridPoint parameter, because function is responsible for 
	// general adjustments, valid for all grid points

	virtual void PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
	{
		SetShapeRect( ppOffset, ppSize );
	}

	void SetShapeEmpty()
	{
		m_rect = PixelRect::ZERO_VAL( );
	}

	PixelRectSize SetMinSize( PixelRectSize const rect )
	{
		return m_minSize = rect;
	}

	PixelRectSize SetMinSize( PIXEL const iWidth, PIXEL const iHeight )
	{
		return SetMinSize( PixelRectSize( iWidth, iHeight ) );
	}

	PIXEL GetFieldSize()
	{
		return m_textDisplay.GetFieldSize( );
	}

	bool IsNotEmpty()
	{
		return m_rect.IsNotEmpty( );
	}

	bool SetShapeRect( PixelPoint const ppOffset, PixelRectSize const ppSize )
	{
		bool bRes = ( ppSize.GetX() >= m_minSize.GetX() ) && 
			        ( ppSize.GetY() >= m_minSize.GetY() );
		if ( bRes )
			m_rect = PixelRect( ppOffset, ppSize );
		else 
			m_rect = PixelRect::ZERO_VAL( );
		return bRes;
	}

	PixelPoint const GetShapePos( )
	{
		return m_rect.GetStartPoint( );
	}

protected:

	PixelRectSize const getShapeSize( )
	{
		return m_rect.GetSize( );
	}

	PIXEL const getShapeWidth()
	{
		return m_rect.GetWidth( );
	}

	PIXEL const getShapeHeight()
	{
		return m_rect.GetHeight( );
	}

	virtual void FillBuffer( EvolutionCore const * const, GridPoint const ) { };

	TextDisplay & m_textDisplay;

private:
	PixelRect     m_rect;      // position is relative to GridPointShape
 	PixelRectSize m_minSize;   // Smallest possible size of Shape in pixels 
};
