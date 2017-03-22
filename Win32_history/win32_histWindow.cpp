
// win32_histWindow.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "strsafe.h"
#include "win32_util.h"
#include "HistorySystem.h"
#include "historyIterator.h"
#include "win32_genDisplayWindow.h"
#include "win32_histWindow.h"

HistWindow::HistWindow( ) :
    BaseWindow( ),
    m_pHistSys( nullptr ),
    m_pHistIter( nullptr ),
    m_pGenDisplay( nullptr ),
    m_trackStruct( { sizeof( TRACKMOUSEEVENT ), TME_LEAVE, nullptr, 0L } ),
    m_genSelected( )
{ }

void HistWindow::Start
(
    HWND      const hWndParent,
    HistorySystem * pHistSys
)
{
    HWND const hWnd = StartBaseWindow
    (
        hWndParent,
        CS_OWNDC | CS_DBLCLKS,
        L"ClassHistWindow",
        WS_POPUP | WS_CLIPSIBLINGS | WS_VISIBLE
    );

    Util::AddWindowStyle( hWnd, WS_EX_STATICEDGE );
    m_trackStruct.hwndTrack = hWnd;

    m_pHistSys = pHistSys;
    m_pHistIter = m_pHistSys->CreateHistoryIterator( );
    Util::MakeLayered( hWnd, TRUE, 0, 100 );  // window is completely opaque (alpha = 100)

    m_pGenDisplay = new GenDisplayWindow( );
    m_pGenDisplay->StartGenDisplayWindow( GetWindowHandle( ) );
}

HistWindow::~HistWindow( )
{
    //lint -e1551   won't throw exception
    delete m_pHistIter;
    delete m_pGenDisplay;
    //lint +e1551
    m_pHistSys = nullptr;
    m_pHistIter = nullptr;
    m_pGenDisplay = nullptr;
}

RECT HistWindow::GetGenerationRect( HIST_GENERATION const gen ) const
{
    return GetGenerationRect( gen, gen + 1 );
}

RECT HistWindow::GetGenerationRect  // position is relative to client area
(
    HIST_GENERATION const genLo,
    HIST_GENERATION const genHi
) const
{
    PixelRect       const rect = GetClRect( );
    long            const lPixSize = rect.right;
    HIST_GENERATION const genNrOfGens = GetHistComp( )->GetNrOfGenerations( );
    assert( genNrOfGens  > 0 );

    RECT pixRect;
    pixRect.left  = MulDiv( genLo.GetLong( ), lPixSize, genNrOfGens.GetLong( ) );
    pixRect.right = MulDiv( genHi.GetLong( ), lPixSize, genNrOfGens.GetLong( ) );
    pixRect.top = 0;
    pixRect.bottom = rect.bottom;

    if ( pixRect.left == pixRect.right )
        ++pixRect.right;

    return pixRect;
}

HIST_GENERATION HistWindow::getGenFromXpos( LPARAM const lParam ) const
{
    long            const lXpos = GET_X_LPARAM( lParam );
    long            const lWidth = GetClientWindowWidth( );
    long            const lNrOfGens = m_pHistSys->GetNrOfGenerations( ).GetLong( );
    HIST_GENERATION const genRes = ( lXpos * lNrOfGens ) / lWidth;

    assert( genRes >= 0 );

    return genRes;
}

void HistWindow::dispGenerationWindow( ) const
{
    int        const iGenDispWidth  = 50;
    int        const iGenDispHeight = 20;
    long       const lClientWidth   = GetClientWindowWidth( );
    RECT       const pixRectGen     = GetGenerationRect( m_genSelected ); // position is relative to client area
    PixelPoint const ptClientPos    = GetClientAreaPos( );                // position of client area origin in screen coordinates
    int        const iYpos          = ptClientPos.y - iGenDispHeight;
    int              iXpos          = ( pixRectGen.left + pixRectGen.right - iGenDispWidth ) / 2 + ptClientPos.x;

    iXpos = max( iXpos, ptClientPos.x );                                 // do not leave client area on left
    iXpos = min( iXpos, ptClientPos.x + lClientWidth - iGenDispWidth );  // or right side

    m_pGenDisplay->Move( iXpos, iYpos, iGenDispWidth, iGenDispHeight, FALSE );
    m_pGenDisplay->DisplayGenerationBelowCursor( m_genSelected );
    m_pGenDisplay->Show( TRUE );
}

void HistWindow::paintGeneration( HDC const hDC, HIST_GENERATION const gen, COLORREF const col ) const
{
    assert( gen >= 0 );
    SetBkColor( hDC, col );
    Util::FastFill( hDC, GetGenerationRect( gen ) );
}

void HistWindow::paintPixelPos( HDC const hDC, long const lPixPos ) const
{
    long            const lClientWidth = GetClientWindowWidth( );
    HIST_GENERATION const genNrOfGens = m_pHistSys->GetNrOfGenerations( );
    HIST_GENERATION const genMin = MulDiv( lPixPos,     genNrOfGens.GetLong( ), lClientWidth ) + 1;
    HIST_GENERATION       genMax = MulDiv( lPixPos + 1, genNrOfGens.GetLong( ), lClientWidth );
    BOOL                  bFoundPos = FALSE;  // Found a pixel pos representing an existing history entry
    BOOL                  bFoundNeg = FALSE;  // Found a pixel pos representing a missing history entry

    if ( genMax < genMin )
        genMax = genMin;

    while ( m_pHistIter->GetCurrentGeneration( ) >= genMin )  // move backwards before genMin
        ( void )m_pHistIter->Set2Senior( );

    do
    {
        HIST_GENERATION gen = m_pHistIter->GetCurrentGeneration( );

        if ( ( genMin <= gen ) && ( gen <= genMax ) )  // gen maps to lPixPos
            bFoundPos = TRUE;

        if ( m_pHistIter->Set2Junior( ) == -1 )
            break;

        if ( m_pHistIter->GetCurrentGeneration( ) > gen + 1 )
            bFoundNeg = TRUE;       // gap found

    } while ( m_pHistIter->GetCurrentGeneration( ) <= genMax );  // move forwards after genMax

    SetBkColor
    (
        hDC,
        bFoundPos
        ? ( bFoundNeg ? CLR_MIX : CLR_DARK )
        : CLR_BACK
    );

    Util::FastFill( hDC, { lPixPos, 0, lPixPos + 1,  GetClientWindowHeight( ) } );
}

void HistWindow::paintHighlightGenerations( HDC const hDC ) const
{
    HIST_GENERATION const genActive = m_pHistSys->GetCurrentGeneration( );

    paintGeneration( hDC, genActive, CLR_RED );

    if ( ( m_genSelected >= 0L ) && ( m_genSelected != genActive ) )
    {
        paintGeneration( hDC, m_genSelected, CLR_YELLOW );
    }

    {
        HIST_GENERATION const genDemanded = GetGenDemanded( );
        if ( ( genActive != genDemanded ) && ( m_pHistSys->IsInHistoryMode( ) ) && !genDemanded.IsInfinite( ) )
            paintGeneration( hDC, genDemanded, CLR_GREEN );
    }
}

void HistWindow::DoPaint( HDC const hDC )
{
    long const lPixSize = GetClientWindowWidth( );

    if ( m_pHistSys->GetNrOfGenerations( ) <= 1 )
        return;

    if ( m_pHistSys->GetNrOfGenerations( ) < lPixSize )
    {
        int iRun = m_pHistIter->Set2Oldest( );

        for ( HIST_GENERATION genRun = 0; iRun != -1; ++genRun )
        {
            BOOL bGenRunInHistory = genRun >= m_pHistIter->GetCurrentGeneration( );
            paintGeneration( hDC, genRun, bGenRunInHistory ? CLR_DARK : CLR_BACK );
            if ( bGenRunInHistory )
                iRun = m_pHistIter->Set2Junior( );
        }
    }
    else // more generations than pixels
    {
        (void)m_pHistIter->Set2Oldest( );

        for ( long lPixPos = 0; lPixPos < lPixSize; ++lPixPos )
        {
            paintPixelPos( hDC, lPixPos );
        }
    }

    paintHighlightGenerations( hDC );
}

LRESULT HistWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch ( message )
    {

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint( &ps );
        DoPaint( hDC );
        (void)EndPaint( &ps );

        if ( ( CrsrInClientRect( ) || IsCaptured( ) ) && ( m_genSelected >= 0 ) )
            dispGenerationWindow( );
        else
            m_pGenDisplay->Show( FALSE );

        return 0;
    }

    case WM_MOUSEMOVE:
        (void)TrackMouseEvent( &m_trackStruct );
        m_genSelected = getGenFromXpos( lParam );
        if ( wParam & MK_LBUTTON )                // Left mouse button
            PostGotoGeneration( m_genSelected );
        else
            Invalidate( FALSE );   // Redraw, do not erase background
        return 0;

    case WM_MOUSELEAVE:
        m_pGenDisplay->Show( FALSE );
        m_genSelected = -1L;
        Invalidate( FALSE );   // Redraw, do not erase background
        return 0;

    case WM_LBUTTONDOWN:
        m_genSelected = getGenFromXpos( lParam );
        PostGotoGeneration( m_genSelected );
        (void)SetCapture( );
        (void)SetFocus( );
        Invalidate( FALSE );   // Redraw, do not erase background
        return 0;

    case WM_LBUTTONUP:
        (void)ReleaseCapture( );
        return 0;

    default:
        break;
    }

    return DefWindowProc( message, wParam, lParam );
}
