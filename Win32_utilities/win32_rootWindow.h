// win32_rootWindow.h : 
//

#pragma once

#include "Windowsx.h"
#include "commctrl.h"
#include "pixelRect.h"
#include "pixelPoint.h"
#include "ObserverInterface.h"
#include "win32_util.h"

class Observer;

class RootWindow: public ObserverInterface
{
public:

    RootWindow( );
    virtual ~RootWindow( );

    HWND GetWindowHandle( ) const { return m_hwnd; };

    LONG          const GetWindowTop( )            const { return Util::GetWindowTop           ( m_hwnd ); }
    LONG          const GetWindowWidth( )          const { return Util::GetWindowWidth         ( m_hwnd ); }
    LONG          const GetWindowHeight( )         const { return Util::GetWindowHeight        ( m_hwnd ); }
    PixelPoint    const GetWindowPos( )            const { return Util::GetWindowPos           ( m_hwnd ); }
    PixelPoint    const GetWindowSize( )           const { return Util::GetWindowSize          ( m_hwnd ); }
    PixelPoint    const GetClientAreaPos( )        const { return Util::GetClientAreaPos       ( m_hwnd ); }
    PixelRectSize const GetClRectSize( )           const { return Util::GetClRectSize          ( m_hwnd ); }
    PixelRect     const GetClPixelRect( )          const { return Util::GetClPixelRect         ( m_hwnd ); }
    RECT          const GetClRect( )               const { return Util::GetClRect              ( m_hwnd ); }
    LONG          const GetClientWindowHeight( )   const { return Util::GetClientWindowHeight  ( m_hwnd ); }
    LONG          const GetClientWindowWidth( )    const { return Util::GetClientWindowWidth   ( m_hwnd ); }
    BOOL          const CrsrInClientRect( )        const { return Util::CrsrInClientRect       ( m_hwnd ); }
    PixelPoint    const GetRelativeCrsrPosition( ) const { return Util::GetRelativeCrsrPosition( m_hwnd ); }
    PixelPoint    const GetClRectCenter( )         const { return Util::GetClRectCenter        ( m_hwnd ); }

    BOOL          const IsWindowVisible( )            const { return ::IsWindowVisible( m_hwnd ); }
    HWND          const SetCapture( )                 const { return ::SetCapture     ( m_hwnd ); }
    HWND          const SetFocus( )                   const { return ::SetFocus       ( m_hwnd ); }
    HWND          const GetDlgItem( int const iItem ) const { return ::GetDlgItem     ( m_hwnd, iItem ); }
    BOOL          const IsCaptured( )                 const { return GetCapture( ) == m_hwnd; }

	void SetDisplayRate( INT const iRate ) { m_iDisplayRate = iRate; }
	
    void Show( tBoolOp const op ) const { Util::Show( m_hwnd, op ); }
    void Show( BOOL    const b  ) const { Util::Show( m_hwnd, b  ); }

    HDC  BeginPaint( LPPAINTSTRUCT lpPaint ) const { return ::BeginPaint( m_hwnd, lpPaint ); }
    BOOL EndPaint  ( LPPAINTSTRUCT lpPaint ) const { return ::EndPaint  ( m_hwnd, lpPaint ); }
    
	LRESULT const PostMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::PostMessage( m_hwnd, msg, wParam, lParam );
    }

    LRESULT const SendMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::SendMessage( m_hwnd, msg, wParam, lParam );
    }

    LRESULT const SendNotifyMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::SendNotifyMessage( m_hwnd, msg, wParam, lParam );
    }

    LRESULT const SendDlgItemMessage( int const iItem, UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::SendDlgItemMessage( m_hwnd, iItem, msg, wParam, lParam );
    }

    LRESULT const DefWindowProc( UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::DefWindowProc( m_hwnd, msg, wParam, lParam );
    }

    void CheckRadioButton( int iFirst, int iLast, int iCheck ) const
    {
        (void)::CheckRadioButton( m_hwnd, iFirst, iLast, iCheck );
    }

    void Move( int const xPos, int const yPos, int const width, int const height, BOOL const bRedraw )
    {
        (void)MoveWindow( m_hwnd, xPos, yPos, width, height, bRedraw );
    }

    void Invalidate( BOOL const bRedraw ) 
    { 
		if ( m_hwnd != nullptr )
			(void)InvalidateRect( m_hwnd, nullptr, bRedraw );
    }

    void FillBackground( HDC const hDC, COLORREF const col ) const
    {
        SetBkColor( hDC, col );
        Util::FastFill( hDC, GetClRect( ) );
    }

	LONG GetTrackBarPos( INT const idTrackbar ) const
	{
		return static_cast<LONG>( SendDlgItemMessage( idTrackbar, TBM_GETPOS, 0, 0 ) ); 
	}

	void SetTrackBarPos( INT const idTrackbar, LONG const lPos ) const
	{
		(void)SendDlgItemMessage
		(   
			idTrackbar, TBM_SETPOS, 
			static_cast<WPARAM>( TRUE ),                   // redraw flag 
			static_cast<LPARAM>( lPos )
		); 
	}

	void SetTrackBarRange( INT const idTrackbar, LONG const lMin, LONG const lMax ) const
	{
		(void)SendDlgItemMessage
		( 
			idTrackbar, 
			TBM_SETRANGEMIN, 
			static_cast<WPARAM>( FALSE ),                   // redraw flag 
			static_cast<LPARAM>( lMin ) 
		);
		(void)SendDlgItemMessage
		( 
			idTrackbar, 
			TBM_SETRANGEMAX, 
			static_cast<WPARAM>( TRUE ),                   // redraw flag 
			static_cast<LPARAM>( lMax ) 
		);
	}

	LRESULT PostCommand2Application( WPARAM const wParam, LPARAM const lParam )
	{
		return ::PostMessage( m_hwndApp, WM_COMMAND, wParam, lParam );
	}

	// GetCrsrPosFromLparam - Windows delivers cursor position in coordinate system with origin in left upper corner
	//                        transform to coord system with origin in left lower corner

	PixelPoint const GetCrsrPosFromLparam( LPARAM const lParam ) const
	{
		PixelPoint ptCrsr( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
		Util::UpsideDown( m_hwnd, & ptCrsr ); 
		return ptCrsr;
	}

	virtual void Notify( );

protected:

	void SetWindowHandle( HWND const );

private:

    static void CALLBACK TimerProc( void * const, BOOL const );

    void invalidate( );
    void startTimer( DWORD const );

    HWND   m_hwnd;
	HWND   m_hwndApp;
    INT    m_iDisplayRate; // in milliseconds
    HANDLE m_hTimer;
    BOOL   m_bTimerActive;
    BOOL   m_bDirty;
};

BOOL RootWinIsReady( RootWindow const * );
