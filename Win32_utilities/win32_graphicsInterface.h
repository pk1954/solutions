// GraphicsInterface.h
//
// Utilities

#pragma once

#include <string.h>
#include "BoolOp.h"
#include "PixelTypes.h"

using COLORREF = unsigned long;

class GraphicsInterface
{
public:

    virtual bool      StartFrame( HWND const, HDC const ) = 0;
    virtual void      SetFontSize( PIXEL const ) = 0;
    virtual void      AddIndividual( PixelPoint const, COLORREF const, float const ) = 0;    
    virtual void      AddBackGround( PixelPoint const, COLORREF const, float const ) = 0;    
    virtual void      RenderIndividuals( ) = 0;
    virtual void      RenderBackground( ) = 0;
    virtual void      RenderTranspRect( PixelRect const &, unsigned int const, COLORREF const ) = 0;
    virtual void      DisplayGraphicsText( PixelRect const &, std::wstring const & ) = 0;
    virtual PixelRect CalcGraphicsRect( std::wstring const & ) = 0;
	virtual void      EndFrame( ) = 0;
    virtual void      SetStripMode( tBoolOp const ) = 0; 
    virtual bool      GetStripMode( ) = 0; 
};
