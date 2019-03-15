// GDI_driver.h
//
// GDI

#pragma once

#include <string.h>
#include "windows.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_graphicsInterface.h"

class GDI_driver: public GraphicsInterface
{
public:

    GDI_driver();
    ~GDI_driver();

    virtual bool      StartFrame( HWND const, HDC const );
    virtual void      SetFontSize( PIXEL const );
    virtual void      AddIndividual( PixelPoint const, COLORREF const, float const );    
    virtual void      AddBackGround( PixelPoint const, COLORREF const, float const );    
    virtual void      RenderIndividuals( );
    virtual void      RenderBackground( );
    virtual void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const );
    virtual void      DisplayGraphicsText( PixelRect const &, std::wstring const & );
    virtual PixelRect CalcGraphicsRect( std::wstring const & );
	virtual void      EndFrame( );
	 
    virtual void SetStripMode( tBoolOp const bOp ) 
	{ 
		ApplyOp( m_bStripMode, bOp ); 
	};

    virtual bool GetStripMode( ) 
	{ 
		return m_bStripMode; 
	};

private:
	HDC  m_hDC;
    bool m_bStripMode;
};
