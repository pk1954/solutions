// win32_status.h 
//
// Win32_appFramework

#pragma once

#include <string>
#include <vector>
#include "PixelTypes.h"
#include "win32_rootWindow.h"

class HistorySystem;
class WorkThreadInterface;

using std::wstring;

class StatusBar : public RootWindow
{
public:
	StatusBar();

	void Start
	( 
		HWND                        const, 
		HistorySystem       const * const, 
		WorkThreadInterface const * const
	);
	void  Stop( );

	PIXEL GetHeight( ) const;
    void  Resize( ) const;

    HWND WINAPI AddStaticControl( LPCTSTR );
    HWND WINAPI AddButton       ( LPCTSTR const, HMENU const, DWORD const  );
    HWND WINAPI AddTrackBar     ( HMENU );

	void AddCustomControl( PIXEL const );

	int  NewPart( );
	void LastPart( );

	void DisplayInPart( int const, std::wstring const & );

private:

	HWND WINAPI addControl( LPCTSTR, LPCTSTR, DWORD, HMENU );

	std::vector< PIXEL > m_statWidths;

	PIXEL m_pixClientHeight;
    PIXEL m_pixBorderX;
    PIXEL m_pixBorderY;
    PIXEL m_pixPosX;

	HistorySystem       const * m_pHistorySystem;
	WorkThreadInterface const * m_pWorkThreadInterface;

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
