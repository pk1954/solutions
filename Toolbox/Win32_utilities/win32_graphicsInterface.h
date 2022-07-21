// GraphicsInterface.h
//
// Win32_utilities

#pragma once

#include <string.h>
#include "windows.h"
#include "PixelTypes.h"

import BoolOp;

using COLORREF = unsigned long;

class GraphicsInterface
{
public:

	virtual void      Initialize( HWND const, ULONG const, ULONG const, BOOL const ) = 0;
	virtual bool      StartFrame( HWND const, HDC const ) = 0;
    virtual void      SetFontSize( PIXEL const ) = 0;
    virtual void      AddIndividual( PixelPoint const, COLORREF const, float const ) = 0;    
    virtual void      AddBackGround( PixelPoint const, COLORREF const, float const ) = 0;    
    virtual void      RenderForegroundObjects( ) = 0;
    virtual void      RenderBackground( ) = 0;
    virtual void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const ) = 0;
    virtual void      DisplayGraphicsText( PixelRect const &, std::wstring const &, DWORD const, COLORREF const ) = 0;
    virtual PixelRect CalcGraphicsRect( std::wstring const & ) = 0;
	virtual void      EndFrame( HWND const ) = 0;
	virtual void      ShutDown( ) = 0;

	virtual void	  AddfPixelLine( fPixelPoint const &, fPixelPoint const &, fPixel const, COLORREF const ) = 0;
	virtual void      DrawPolygon( int const, fPixelPoint const, COLORREF const, fPixel const ) = 0;
	virtual void      StartPipeline( fPixelPoint const &, fPixelPoint const &, fPixel const, COLORREF const ) = 0;
	virtual void      AddPipelinePoint( fPixelPoint const &, COLORREF const ) = 0;
	virtual void      RenderPipeline( ) = 0;

	void SetStripMode(tBoolOp const bOp) 
	{ 
		ApplyOp(m_bStripMode, bOp); 
	};

	bool GetStripMode() 
	{ 
		return m_bStripMode; 
	};

protected:
	bool m_bStripMode;
};
