// win32_rootWindow.h : 
//

#pragma once

#include "windows.h"
#include "commctrl.h"
#include "pixelRect.h"
#include "pixelPoint.h"
#include "win32_util.h"

class RootWindow
{
public:

    RootWindow( ) : m_hWnd( nullptr ) { }

    virtual ~RootWindow( ) { m_hWnd = nullptr; }

    HWND GetWindowHandle( ) const { return m_hWnd; };

    LONG          const GetWindowTop( )            const { return Util::GetWindowTop           ( m_hWnd ); }
    LONG          const GetWindowWidth( )          const { return Util::GetWindowWidth         ( m_hWnd ); }
    LONG          const GetWindowHeight( )         const { return Util::GetWindowHeight        ( m_hWnd ); }
    PixelPoint    const GetWindowPos( )            const { return Util::GetWindowPos           ( m_hWnd ); }
    PixelPoint    const GetWindowSize( )           const { return Util::GetWindowSize          ( m_hWnd ); }
    PixelPoint    const GetClientAreaPos( )        const { return Util::GetClientAreaPos       ( m_hWnd ); }
    PixelRectSize const GetClRectSize( )           const { return Util::GetClRectSize          ( m_hWnd ); }
    PixelRect     const GetClPixelRect( )          const { return Util::GetClPixelRect         ( m_hWnd ); }
    RECT          const GetClRect( )               const { return Util::GetClRect              ( m_hWnd ); }
    LONG          const GetClientWindowHeight( )   const { return Util::GetClientWindowHeight  ( m_hWnd ); }
    LONG          const GetClientWindowWidth( )    const { return Util::GetClientWindowWidth   ( m_hWnd ); }
    BOOL          const CrsrInClientRect( )        const { return Util::CrsrInClientRect       ( m_hWnd ); }
    PixelPoint    const GetRelativeCrsrPosition( ) const { return Util::GetRelativeCrsrPosition( m_hWnd ); }
    PixelPoint    const GetClRectCenter( )         const { return Util::GetClRectCenter        ( m_hWnd ); }

    BOOL          const IsWindowVisible( )            const { return ::IsWindowVisible( m_hWnd ); }
    HWND          const SetCapture( )                 const { return ::SetCapture     ( m_hWnd ); }
    HWND          const SetFocus( )                   const { return ::SetFocus       ( m_hWnd ); }
    HWND          const GetDlgItem( int const iItem ) const { return ::GetDlgItem     ( m_hWnd, iItem ); }
    BOOL          const IsCaptured( )                 const { return GetCapture( ) == m_hWnd; }

    PixelPoint    const GetCrsrPosFromLparam( LPARAM const ) const;

    void Show( tBoolOp const op ) const { Util::Show( m_hWnd, op ); }
    void Show( BOOL    const b  ) const { Util::Show( m_hWnd, b  ); }

    HDC  BeginPaint( LPPAINTSTRUCT lpPaint ) const { return ::BeginPaint( m_hWnd, lpPaint ); }
    BOOL EndPaint  ( LPPAINTSTRUCT lpPaint ) const { return ::EndPaint  ( m_hWnd, lpPaint ); }
    
    LRESULT PostCommand2Application( WPARAM const, LPARAM const );

	LRESULT const PostMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::PostMessage( m_hWnd, msg, wParam, lParam );
    }

    LRESULT const SendMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::SendMessage( m_hWnd, msg, wParam, lParam );
    }

    LRESULT const SendNotifyMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::SendNotifyMessage( m_hWnd, msg, wParam, lParam );
    }

    LRESULT const SendDlgItemMessage( int const iItem, UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::SendDlgItemMessage( m_hWnd, iItem, msg, wParam, lParam );
    }

    LRESULT const DefWindowProc( UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::DefWindowProc( m_hWnd, msg, wParam, lParam );
    }

    void CheckRadioButton( int iFirst, int iLast, int iCheck ) const
    {
        (void)::CheckRadioButton( m_hWnd, iFirst, iLast, iCheck );
    }

    void Move( int const xPos, int const yPos, int const width, int const height, BOOL const bRedraw )
    {
        (void)MoveWindow( m_hWnd, xPos, yPos, width, height, bRedraw );
    }

    void Invalidate( BOOL const bRedraw ) 
    { 
        (void)InvalidateRect( m_hWnd, nullptr, bRedraw );
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

protected:
    void SetWindowHandle( HWND hwnd ) { m_hWnd = hwnd; };

private:

    HWND m_hWnd;
};

BOOL RootWinIsReady( RootWindow const * );
