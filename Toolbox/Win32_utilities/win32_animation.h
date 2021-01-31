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

typedef void (* APP_PROC)(HWND const, bool const);

template <typename ANIMATION_OBJ>
class Animation
{
public:

    Animation
    ( 
        HWND      const hwnd,
        ANIMATION_OBJ & actual,
        APP_PROC  const appProc
    )
      : m_hwnd(hwnd),
        m_actual(actual),
        m_pAppProc(appProc)
    {
        m_idTimer = GetRootWindow(hwnd)->AddSlot( reinterpret_cast<void *>(this) );
    }

    void Start( ANIMATION_OBJ const origin, ANIMATION_OBJ const target )
    {
        m_start          = origin;
        m_target         = target;
        m_actual         = m_start;
        m_bTargetReached = false;
        m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        SetTimer( m_hwnd, m_idTimer, DEFAULT_MILLISECS, timerProc );
    }

    bool          const TargetReached() const { return m_bTargetReached; }
    ANIMATION_OBJ const GetTarget()     const { return m_target; }

private:
    static unsigned int const DEFAULT_NR_OF_STEPS { 20 };
    static unsigned int const DEFAULT_MILLISECS   { 50 };

    void next( )
    {
        auto pRootWin { GetRootWindow( m_hwnd ) };
        m_bTargetReached = m_smoothMove.Next();
        m_actual = m_start + (m_target - m_start) * m_smoothMove.GetPos();
        if ( m_bTargetReached )
            KillTimer( m_hwnd, m_idTimer );
        pRootWin->Notify( false );
        if (m_pAppProc)
            (*m_pAppProc)( m_hwnd, m_bTargetReached );
    }

    static void timerProc( HWND hwnd, UINT msgTimer, UINT_PTR idTimer, DWORD msSinceStart )
    {
        auto pAnimation { reinterpret_cast<Animation<ANIMATION_OBJ> *>(GetSlot(hwnd,idTimer)) };
        pAnimation->next();
    }

    friend static void timerProc( HWND, UINT, UINT_PTR, DWORD );

    SmoothMoveFp<float> m_smoothMove;

    HWND            m_hwnd;
    ANIMATION_OBJ & m_actual;
    APP_PROC        m_pAppProc;
    bool            m_bTargetReached { false };
    UINT_PTR        m_idTimer { 0 };
    ANIMATION_OBJ   m_start;
    ANIMATION_OBJ   m_target;
};
