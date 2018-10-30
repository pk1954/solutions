// win32_gridPointShape.cpp
//

#pragma once

#include <sstream> 
#include "d3d_buffer.h"
#include "GridPoint.h"
#include "EvolutionCore.h"
#include "win32_rectShape.h"

class GridPointShape : public RectShape
{
public:
	GridPointShape( D3dBuffer * pD3dBuffer, wostringstream & wBuffer, EvolutionCore * const pCore ) :
		m_pD3dBuffer( pD3dBuffer ),
		m_wBuffer( wBuffer ),
		m_pCore( pCore )
	{}

	void Draw( GridPoint const gp, PixelPoint const & inheritedOffset )
	{
		m_wBuffer.str( wstring() );
		m_wBuffer.clear();
		FillBuffer( gp );
		m_pD3dBuffer->D3D_DrawText( GetRect( inheritedOffset ), m_wBuffer.str( ), CLR_WHITE );
	}

	virtual void Highlight( PixelPoint const & offset ) const
	{}

protected:

	virtual void FillBuffer( GridPoint const ) = 0;

    D3dBuffer     *       m_pD3dBuffer;
	wostringstream      & m_wBuffer;
    EvolutionCore * const m_pCore;
};   

