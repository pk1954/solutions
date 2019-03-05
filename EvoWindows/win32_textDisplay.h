// win32_textDisplay.h

#pragma once

#include <sstream> 
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "GraphicsInterface.h"

class TextDisplay
{
public:
	TextDisplay
	( 
		GraphicsInterface   & graphicsInterface, 
		std::wostringstream & wBuffer,
		PixelCoordinates    & pixelCoordinates,
		EvolutionCore       & core
	) :
		m_graphics( graphicsInterface ),
		m_wBuffer( wBuffer ),
		m_pixelCoordinates( pixelCoordinates ),
		m_core( core )
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

	EvolutionCore & Core()
	{
		return m_core;
	}

	Strategy::Id GetStrategyId( GridPoint const gp ) const
	{
		return  m_core.GetStrategyId( gp );
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
    EvolutionCore       & m_core;
};
