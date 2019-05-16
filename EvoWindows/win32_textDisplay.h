// win32_textDisplay.h

#pragma once

#include <sstream> 
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "win32_graphicsInterface.h"

class TextDisplay
{
public:
	TextDisplay( )
    :	m_pGraphics( nullptr ),
		m_pBuffer( nullptr ),
		m_pPixelCoordinates( nullptr )
	{ }

	void Start
	( 
		GraphicsInterface   * pGgraphicsInterface, 
		std::wostringstream * pBuffer,
		PixelCoordinates    * PpixelCoordinates
	)
	{
		m_pGraphics         = pGgraphicsInterface;
		m_pBuffer           = pBuffer;
		m_pPixelCoordinates = PpixelCoordinates;
	}

	void Clear()
	{
		m_pBuffer->str( std::wstring() );
		m_pBuffer->clear();
	}

	std::wostringstream & Buffer() 
	{
		return * m_pBuffer;
	}

	PIXEL GetFieldSize( ) const 
	{
		return m_pPixelCoordinates->GetFieldSize();
	}

	PixelPoint GetOffset( GridPoint const gp )
	{
		return m_pPixelCoordinates->Grid2PixelPos( gp );
	}

	PixelPoint GetCenterOffset( GridPoint const gp )
	{
		return m_pPixelCoordinates->Grid2PixelPosCenter( gp );
	}

	PixelRectSize CalcRectSize( )
	{
		return m_pGraphics->CalcGraphicsRect( m_pBuffer->str( ) ).GetSize( );
	}

	void DisplayText( PixelRect const & rect )
	{
		m_pGraphics->DisplayGraphicsText( rect, m_pBuffer->str( ) );
	}

private:
    GraphicsInterface   * m_pGraphics;
	std::wostringstream * m_pBuffer;
	PixelCoordinates    * m_pPixelCoordinates;
};
