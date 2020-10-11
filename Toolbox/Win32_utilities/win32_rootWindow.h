// win32_rootWindow.h : 
//
// Win32_utilities

#pragma once

#include <chrono>
#include "Windowsx.h"
#include "commctrl.h"
#include "PixelTypes.h"
#include "ObserverInterface.h"
#include "win32_tooltip.h"
#include "win32_util.h"

using std::chrono::milliseconds;
using std::function;

inline LONG_PTR GetUserDataPtr( HWND hwnd )               { return GetWindowLongPtr( hwnd, GWLP_USERDATA ); }
inline void     SetUserDataPtr( HWND hwnd, LONG_PTR ptr ) { (void) SetWindowLongPtr( hwnd, GWLP_USERDATA, ptr ); }

class RootWindow : public ObserverInterface
{
public:

    RootWindow( );
    virtual ~RootWindow( );

	void StartRootWindow( function<bool()> const );

    HWND GetWindowHandle( ) const { return m_hwnd; };

    PIXEL         const GetWindowTop( )            const { return Util::GetWindowTop           ( m_hwnd ); }
    PIXEL         const GetWindowWidth( )          const { return Util::GetWindowWidth         ( m_hwnd ); }
    PIXEL         const GetWindowHeight( )         const { return Util::GetWindowHeight        ( m_hwnd ); }
    PixelRectSize const GetWindowSize( )           const { return Util::GetWindowSize          ( m_hwnd ); }
    PixelRectSize const GetClRectSize( )           const { return Util::GetClRectSize          ( m_hwnd ); }
    PixelRect     const GetClPixelRect( )          const { return Util::GetClPixelRect         ( m_hwnd ); }
    PIXEL         const GetClientWindowHeight( )   const { return Util::GetClientWindowHeight  ( m_hwnd ); }
    PIXEL         const GetClientWindowWidth( )    const { return Util::GetClientWindowWidth   ( m_hwnd ); }
    bool          const CrsrInClientRect( )        const { return Util::CrsrInClientRect       ( m_hwnd ); }
    PixelPoint    const GetRelativeCrsrPosition( ) const { return Util::GetRelativeCrsrPosition( m_hwnd ); }
    PixelPoint    const GetClRectCenter( )         const { return Util::GetClRectCenter        ( m_hwnd ); }

	PixelPoint    const Client2Screen( PixelPoint  const & p ) const { return Util::Client2Screen ( m_hwnd, p ); }
	PixelPoint    const Screen2Client( PixelPoint  const & p ) const { return Util::Screen2Client ( m_hwnd, p ); }

	bool          const IsInClientRect( PixelPoint const & p ) const { return Util::IsInClientRect( m_hwnd, p ); }
	bool          const IsInClientRect( PixelRect  const & r ) const { return Util::IsInClientRect( m_hwnd, r ); }

	bool          const IsWindowVisible( )            const { return ::IsWindowVisible( m_hwnd ); }
	HWND          const SetCapture( )                 const { return ::SetCapture     ( m_hwnd ); }
    HWND          const SetFocus( )                   const { return ::SetFocus       ( m_hwnd ); }
    HWND          const GetDlgItem( int const iItem ) const { return ::GetDlgItem     ( m_hwnd, iItem ); }
    bool          const IsCaptured( )                 const { return ::GetCapture( ) == m_hwnd; }
	int			  const GetWindowTextLength( )        const { return ::GetWindowTextLength( m_hwnd ); }        

	void SetRefreshRate( milliseconds const );
	
    void Show( tBoolOp const op ) const { Util::Show( m_hwnd, op ); }
    void Show( bool    const b  ) const { Util::Show( m_hwnd, b  ); }

    HDC  BeginPaint( LPPAINTSTRUCT lpPaint ) const { return ::BeginPaint( m_hwnd, lpPaint ); }
    bool EndPaint  ( LPPAINTSTRUCT lpPaint ) const { return ::EndPaint  ( m_hwnd, lpPaint ); }

	void SetTrackBarPos( INT const, LONG const ) const;
	void SetTrackBarRange( INT const, LONG const, LONG const ) const;

	void const DestroyWindow( ) 
	{ 
		::DestroyWindow( m_hwnd );
		m_hwnd = nullptr;
	}

	HWND CreateBalloonToolTip( int const id, LPWSTR const t ) 
	{
		return ::CreateBalloonToolTip( m_hwnd, id, t );	
	}

	HWND CreateStdToolTip( int const id, LPWSTR const t ) 
	{ 
		return ::CreateStdToolTip( m_hwnd, id, t );	
	}

	HWND CreateWindowToolTip( LPWSTR const t ) 
	{ 
		return ::CreateWindowToolTip( m_hwnd, t );	
	}

	HWND CreateRectToolTip( int const id, PixelRect * const pRect, LPWSTR const t ) 
	{ 
		return ::CreateRectToolTip( m_hwnd, id, pRect, t );	
	}

	HBITMAP const CreateCompatibleBitmap( HDC const hDC )
	{
		PixelRectSize rectSize = GetClRectSize( );
		return ::CreateCompatibleBitmap( hDC, rectSize.GetXvalue(), rectSize.GetYvalue() );
	}
	
	LRESULT const PostMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::PostMessage( m_hwnd, msg, wParam, lParam );
    }

    LRESULT const SendMessage( UINT const msg, WPARAM const wParam, LPARAM const lParam ) const
    {
        return ::SendMessage( m_hwnd, msg, wParam, lParam );
    }

    LRESULT const SendDlgItemMessage( int const iItem, UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::SendDlgItemMessage( m_hwnd, iItem, msg, wParam, lParam );
    }

	void SetDlgText( int const iItem, wchar_t const * const wstrText )
	{
		Util::SetText( GetDlgItem( iItem ), wstrText );
	}

    LRESULT const DefWindowProc( UINT msg, WPARAM wParam, LPARAM lParam ) const
    {
        return ::DefWindowProc( m_hwnd, msg, wParam, lParam );
    }

	void CheckRadioButton( int iFirst, int iLast, int iCheck ) const
	{
		(void)::CheckRadioButton( m_hwnd, iFirst, iLast, iCheck );
	}

	UINT IsDlgButtonChecked( int iIdButton ) const
	{
		return ::IsDlgButtonChecked( m_hwnd, iIdButton );
	}

	void Move( PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, bool const bRedraw )
    {
        (void)::MoveWindow( m_hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw );
    }

    void Move( PixelPoint const pos, PixelRectSize const size, bool const bRedraw )
    {
        Move( pos.GetX(), pos.GetY(), size.GetX(), size.GetY(), bRedraw );
    }

    void Move( PixelRect const rect, bool const bRedraw )
    {
        Move( rect.GetStartPoint(), rect.GetSize(), bRedraw );
    }

    void Invalidate( bool const bRedraw ) 
    {
		if ( m_hwnd != nullptr )
			(void)::InvalidateRect( m_hwnd, nullptr, bRedraw );
    }

    void FillBackground( HDC const hDC, COLORREF const col ) const
    {
        SetBkColor( hDC, col );
        Util::FastFill( hDC, m_hwnd );
    }

	short GetTrackBarPos( INT const idTrackbar ) const
	{
		return Cast2Short( SendDlgItemMessage( idTrackbar, TBM_GETPOS, 0, 0 ) ); 
	}

	LRESULT PostCommand2Application( WPARAM const wParam, LPARAM const lParam )
	{
		return ::PostMessage( m_hwndApp, WM_COMMAND, wParam, lParam );
	}

	LRESULT SendCommand2Application( WPARAM const wParam, LPARAM const lParam )
	{
		return ::SendMessage( m_hwndApp, WM_COMMAND, wParam, lParam );
	}

	PIXEL const CrsrXpos( LPARAM const lParam ) const
	{
		return PIXEL(GET_X_LPARAM( lParam ) );
	}

	PIXEL const CrsrYpos( LPARAM const lParam ) const
	{
		return PIXEL(GET_Y_LPARAM( lParam ) );
	}

	PixelPoint const GetCrsrPosFromLparam( LPARAM const lParam ) const
	{
		return PixelPoint{ CrsrXpos(lParam), CrsrYpos(lParam) };
	}

	virtual long AddContextMenuEntries( HMENU const ) { return 0L; }
	virtual void Notify( bool const );

	virtual void Trigger( )	{ Invalidate( false ); }

	void ShowRefreshRateDlg( bool const bShow ) { m_bShowRefreshRateDlg = bShow; }

protected:

	void SetWindowHandle( HWND const );

	virtual bool OnCommand( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );
	virtual bool OnSize   ( WPARAM const, LPARAM const );
	virtual void OnClose( );

	virtual bool CommonMessageHandler( UINT const, WPARAM const, LPARAM const );

private:

	class WindowRefreshRate;

	HWND                  m_hwnd                 { nullptr };
	HWND                  m_hwndApp              { nullptr };
	WindowRefreshRate   * m_pRefreshRate         { nullptr };
	tOnOffAuto            m_visibilityMode       { tOnOffAuto::on };
	function<bool()>      m_visibilityCriterion  { nullptr };
	bool                  m_bShowRefreshRateDlg  { true };

	void addWinMenu( HMENU const, std::wstring const ) const;
	void adjustWinMenu( HMENU const ) const;
	void contextMenu( PixelPoint const & );

	void adjustVisibility( tOnOffAuto const onOffAuto )
	{
		if ( m_visibilityCriterion )
			Show( ApplyAutoCriterion( onOffAuto, m_visibilityCriterion ) );
	}

};

RootWindow * GetRootWindow( HWND const );
