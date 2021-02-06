// win32_animation.h
//
// Win32_utilities

#pragma once

#include "windows.h"
#include "MoreTypes.h"
#include "win32_rootWindow.h"
#include "SmoothMoveFp.h"

template <typename T>
T * GetWinPtr( HWND hwnd )
{
    return reinterpret_cast<T *>(GetUserDataPtr( hwnd ));
}

template <typename ANIM_PAR, typename WIN_PAR>
class Animation
{
public:

    using APP_PROC = void (*)(WIN_PAR * const, bool const);

    Animation
    ( 
        HWND     const hwnd,
        ANIM_PAR     & actual,
        APP_PROC const appProc
    )
      : m_hwnd(hwnd),
        m_actual(actual),
        m_pAppProc(appProc),
        m_bTargetReached(false)
    {
        m_idTimer = GetRootWindow(hwnd)->AddSlot( reinterpret_cast<void *>(this) );
    }

    void Start( ANIM_PAR const origin, ANIM_PAR const target )
    {
        m_start          = origin;
        m_target         = target;
        m_actual         = m_start;
        m_bTargetReached = false;
        m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        SetTimer( m_hwnd, m_idTimer, DEFAULT_MILLISECS, timerProc );
    }

    bool     const TargetReached() const { return m_bTargetReached; }
    ANIM_PAR const GetTarget()     const { return m_target; }

private:
    static unsigned int const DEFAULT_NR_OF_STEPS { 20 };
    static unsigned int const DEFAULT_MILLISECS   { 50 };

    void next( )
    {
        auto pRootWin { GetRootWindow(m_hwnd) };
        m_bTargetReached = m_smoothMove.Next();
        m_actual = m_start + (m_target - m_start) * m_smoothMove.GetPos();
        if ( m_bTargetReached )
            KillTimer( m_hwnd, m_idTimer );
        pRootWin->Notify( false );
        if (m_pAppProc)
            (*m_pAppProc)( GetWinPtr<WIN_PAR>(m_hwnd), m_bTargetReached );
    }

    static void timerProc( HWND hwnd, UINT msgTimer, UINT_PTR idTimer, DWORD msSinceStart )
    {
        auto pAnimation { reinterpret_cast<Animation<ANIM_PAR, WIN_PAR> *>(GetSlot(hwnd,idTimer)) };
        pAnimation->next();
    }

    friend void timerProc( HWND, UINT, UINT_PTR, DWORD );

    SmoothMoveFp<float> m_smoothMove;

    HWND       m_hwnd;
    ANIM_PAR & m_actual;
    APP_PROC   m_pAppProc;
    bool       m_bTargetReached;
    UINT_PTR   m_idTimer { 0 };
    ANIM_PAR   m_start   {};
    ANIM_PAR   m_target  {};
};
