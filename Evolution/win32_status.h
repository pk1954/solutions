// win32_status.h 
//

#pragma once

#include <string>
#include "win32_baseDialog.h"
#include "gridPoint.h"

using namespace std;

class EvoController;
class EvolutionModelData;

class StatusBar : public BaseDialog
{
public:
    StatusBar( );
    ~StatusBar();

    void Start( HWND const, EvoController * const, EvolutionModelData * const );

    int  GetHeight( ) const;
    void Resize( ) const;
    void SetSizeTrackBar ( short const ) const;
    void SetSpeedTrackBar( DWORD const ) const;
    void DisplayStatusLine( std::wstring const & );
    void DisplayScriptLine( std::wstring const &, int, std::wstring const & );
    void DisplayCurrentGeneration( );

protected:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

private:
    enum class tPart
    {
        Generation,
        Size,
        Speed,
        ScriptLine,
        Stop
    };

    void   setTrackBarPos( HWND, USHORT )   const;
    short  trackBarPos2FieldSize( USHORT )  const;
    USHORT fieldSize2TrackBarPos( short )   const;
    DWORD  trackBarPos2SpeedDelay( USHORT ) const;
    USHORT speedDelay2TrackBarPos( DWORD )  const; 

    HWND WINAPI createControl      ( LPCTSTR, LPCTSTR, DWORD, HMENU );
    HWND WINAPI createStaticControl( LPCTSTR );
    HWND WINAPI createButton       ( LPCTSTR, HMENU );
    HWND WINAPI createTrackBar     ( );

    HWND WINAPI createSizeControl ( );
    HWND WINAPI createSpeedControl( );

    EvoController      * m_pEvoController;
    EvolutionModelData * m_pModelWork;
    GridPoint            m_gp;
    INT                  m_iClientHeight;
    INT                  m_iBorder;
    INT                  m_iPosX;
    HWND                 m_hWndSize;
    HWND                 m_hWndSpeed;
    wstring              m_wstrGeneration;
    wstring              m_wstrScriptLine;

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
