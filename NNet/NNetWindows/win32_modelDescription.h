// win32_modelDescription.h : 
//
// NNetWindows

#pragma once

#include <string>
#include "ModelDescription.h"
#include "win32_baseWindow.h"

using std::wstring;

class DescriptionWindow : public BaseWindow, public ModelDescription
{
public:
	void Start( HWND const );
	void Stop( );

	virtual void ClearDescription( );
	virtual void SetDescription( wstring const );
	virtual bool GetDescriptionLine( int const, wstring & ) const;


private:
	HWND m_hwndEdit { nullptr };

	virtual void OnPaint  ( ) { };
	virtual bool OnSize   ( WPARAM const, LPARAM const );
	virtual bool OnCommand( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );
};
