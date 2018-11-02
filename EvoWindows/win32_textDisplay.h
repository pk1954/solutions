// win32_textDisplay.h

#pragma once

#include <sstream> 
#include "d3d_buffer.h"

class TextDisplay
{
public:
	TextDisplay
	( 
		D3dBuffer        & d3dBuffer, 
		wostringstream   & wBuffer,
		PixelCoordinates & pixelCoordinates,
		EvolutionCore    & core
	) :
		m_D3dBuffer( d3dBuffer ),
		m_wBuffer( wBuffer ),
		m_pixelCoordinates( pixelCoordinates ),
		m_core( core )
	{ }

	void Clear()
	{
		m_wBuffer.str( wstring() );
		m_wBuffer.clear();
	}

	wostringstream & Buffer() 
	{
		return m_wBuffer;
	}

	EvolutionCore & Core()
	{
		return m_core;
	}

	long GetIndDisplaySize( )
	{
		return (5 * m_pixelCoordinates.GetFieldSize()) / 8;  // use only 5/8 of field size; 
	}

	PixelPoint GetCenterOffset( GridPoint const gp )
	{
		return m_pixelCoordinates.Grid2PixelPosCenter( gp );
	}

	void DrawText( PixelRect const & rect )
	{
		m_D3dBuffer.D3D_DrawText( rect, m_wBuffer.str( ), CLR_WHITE );
	}

private:
    D3dBuffer        & m_D3dBuffer;
	wostringstream   & m_wBuffer;
	PixelCoordinates & m_pixelCoordinates;
    EvolutionCore    & m_core;
};
