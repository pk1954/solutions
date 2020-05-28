// win32_status.h 
//
// NNetWindows

#pragma once

#include <string>
#include <vector>
#include "PixelTypes.h"
#include "win32_rootWindow.h"

class NNetModelWriterInterface;

using std::wstring;

class StatusBar : public RootWindow
{
public:

	void Start( HWND const );
	void Stop( );

	PIXEL GetHeight( ) const;
    void  Resize( ) const;

    HWND WINAPI AddStaticControl( LPCTSTR );
    HWND WINAPI AddButton       ( LPCTSTR const, HMENU const, DWORD const );
    HWND WINAPI AddTrackBar     ( HMENU );

	void AddCustomControl( int const );

	int  NewPart( );
	void LastPart( );

	void DisplayInPart( int const, wstring const & );

private:

	inline static PIXEL const STATUS_BAR_HEIGHT { 22_PIXEL };

	HWND WINAPI addControl( LPCTSTR, LPCTSTR, DWORD, HMENU );

	std::vector< PIXEL > m_statWidths;

	PIXEL m_pixClientHeight { 0_PIXEL };
    PIXEL m_pixBorderX      { 0_PIXEL };
    PIXEL m_pixBorderY      { 0_PIXEL };
    PIXEL m_pixPosX         { 0_PIXEL };

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
