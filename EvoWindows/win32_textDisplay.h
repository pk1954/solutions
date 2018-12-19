// win32_textDisplay.h

#pragma once

#include <sstream> 
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
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

	tStrategyId GetStrategyId( GridPoint const gp ) const
	{
		return  m_core.GetStrategyId( gp );
	}

	short GetFieldSize( ) const 
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

	PixelRect CalcRect( )
	{
		return m_D3dBuffer.D3D_CalcRect( m_wBuffer.str( ) );
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
