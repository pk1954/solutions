// win32_textDisplay.h

#pragma once

#include <sstream> 
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "win32_graphicsInterface.h"

class TextDisplay
{
public:
	TextDisplay
	( 
		GraphicsInterface   & graphicsInterface, 
		std::wostringstream & wBuffer,
		PixelCoordinates    & pixelCoordinates
	) :
		m_graphics( graphicsInterface ),
		m_wBuffer( wBuffer ),
		m_pixelCoordinates( pixelCoordinates )
	{ }

	void Clear()
	{
		m_wBuffer.str( std::wstring() );
		m_wBuffer.clear();
	}

	std::wostringstream & Buffer() 
	{
		return m_wBuffer;
	}

	PIXEL GetFieldSize( ) const 
	{
		return m_pixelCoordinates.GetFieldSize();
	}

	PixelPoint GetOffset( GridPoint const gp )
	{
		return m_pixelCoordinates.Grid2PixelPos( gp );
	}

	PixelPoint GetCenterOffset( GridPoint const gp )
	{
		return m_pixelCoordinates.Grid2PixelPosCenter( gp );
	}

	PixelRectSize CalcRectSize( )
	{
		return m_graphics.CalcGraphicsRect( m_wBuffer.str( ) ).GetSize( );
	}

	void DisplayText( PixelRect const & rect )
	{
		m_graphics.DisplayGraphicsText( rect, m_wBuffer.str( ) );
	}

private:
    GraphicsInterface   & m_graphics;
	std::wostringstream & m_wBuffer;
	PixelCoordinates    & m_pixelCoordinates;
};
