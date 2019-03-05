// GraphicsInterface.h
//
// EvoWindows

#pragma once

#include <string.h>
#include "PixelTypes.h"
#include "win32_util.h"

class GraphicsInterface
{
public:

    virtual BOOL      StartFrame( ) = 0;
    virtual void      SetFontSize( PIXEL const ) = 0;
    virtual void      AddIndividual( PixelPoint const, DWORD const, float const ) = 0;    
    virtual void      AddBackGround( PixelPoint const, DWORD const, float const ) = 0;    
    virtual void      RenderIndividuals( ) = 0;
    virtual void      RenderBackground( ) = 0;
    virtual void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const ) = 0;
    virtual void      DisplayGraphicsText( PixelRect const &, std::wstring const & ) = 0;
    virtual PixelRect CalcGraphicsRect( std::wstring const & ) = 0;
	virtual void      EndFrame( ) = 0;
    virtual void      SetStripMode( tBoolOp const ) = 0; 
    virtual BOOL      GetStripMode( ) = 0; 
};
