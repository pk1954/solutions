// win32_shape.h : 
//
// Shape is defined by the offset of the top left corner
// and the size of the rectangle 

#pragma once

#include "GridPoint.h"
#include "win32_textDisplay.h"
#include "pixelPoint.h"
#include "pixelRect.h"

using namespace std;

class Shape
{
public:
	Shape( TextDisplay & t ) :
		m_textDisplay( t ),
		m_rect   ( PixelRect    ( ) ),
		m_minSize( PixelRectSize( ) )
	{}

	virtual PixelRectSize MinimalSize( )  
	{                                     
		m_textDisplay.Clear();
		FillBuffer( GridPoint::GRID_ORIGIN );
		return SetMinSize( m_textDisplay.CalcRectSize( ) );
	}                                     

	PixelRect const GetAbsoluteShapeRect( GridPoint const gp ) const 
	{
		return m_rect + m_textDisplay.GetOffset( gp );
	}

	long const GetMinWidth ( ) const { return m_minSize.GetWidth();  }
	long const GetMinHeight( ) const { return m_minSize.GetHeight(); }

	virtual Shape const * FindShape  // for all shapes without subshapes
	( 
		PixelPoint const pnt,             
		GridPoint  const gp
	) const
	{
		return m_rect.Includes( pnt ) ? this : nullptr;
	}

	virtual GridPoint GetReferencedGridPoint( GridPoint const gp ) const 
	{ 
		return GridPoint::GP_NULL; 
	}

	virtual void Draw( GridPoint const, PixelPoint const );
	virtual void AddContextMenuEntries( HMENU const ) const {};

	// PrepareShape: Rearrange shape according new size
	// no GridPoint parameter, because function is responsible for 
	// general adjustments, valid for all grid points

	virtual void PrepareShape( PixelPoint const ppOffset, PixelRectSize const ppSize )
	{
		SetShapeRect( ppOffset, ppSize );
	}

	void SetShapeEmpty()
	{
		return m_rect.SetEmpty( );
	}

	PixelRectSize SetMinSize( PixelRectSize const rect )
	{
		return m_minSize = rect;
	}

	PixelRectSize SetMinSize( int const iWidth, int const iHeight )
	{
		return SetMinSize( PixelRectSize( iWidth, iHeight ) );
	}

	short GetFieldSize()
	{
		return m_textDisplay.GetFieldSize( );
	}

	bool IsNotEmpty()
	{
		return m_rect.IsNotEmpty( );
	}

	bool SetShapeRect( PixelPoint const ppOffset, PixelRectSize const ppSize )
	{
		bool bRes = ppSize.Includes( m_minSize );
		if ( bRes )
			m_rect = PixelRect( ppOffset, ppSize );
		else 
			m_rect.SetEmpty( );
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

	long const getShapeWidth()
	{
		return m_rect.GetWidth( );
	}

	long const getShapeHeight()
	{
		return m_rect.GetHeight( );
	}

	virtual void FillBuffer( GridPoint const ) { };

	TextDisplay & m_textDisplay;

private:
	PixelRect     m_rect;      // position is relative to GridPointShape
 	PixelRectSize m_minSize;   // Smallest possible size of Shape in pixels 
};
