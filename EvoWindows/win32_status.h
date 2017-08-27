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
    ~StatusBar( );

    void Start( HWND const, EvoController * const, EvolutionModelData * const );
	void SetSimuMode( BOOL const );

    int  GetHeight( ) const;
    void Resize( ) const;
    void SetSizeTrackBar ( short const ) const;
    void SetSpeedTrackBar( DWORD const ) const;
    void ClearStatusLine( );
    void DisplayStatusLine( std::wstring const & );
    void DisplayScriptLine( std::wstring const &, int, std::wstring const & );
    void DisplayCurrentGeneration( );

private:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

    enum class tPart
    {
        Generation,
		Mode,
        Size,
        SimuEdit,
        ScriptLine,
        Stop
    };

    void   setTrackBarPos( INT const, USHORT const ) const;
    short  trackBarPos2FieldSize ( USHORT const )    const;
    USHORT fieldSize2TrackBarPos ( SHORT  const )    const;
    DWORD  trackBarPos2SpeedDelay( USHORT const )    const;
    USHORT speedDelay2TrackBarPos( DWORD  const )    const; 

    HWND WINAPI createControl      ( LPCTSTR, LPCTSTR, DWORD, HMENU );
    HWND WINAPI createStaticControl( LPCTSTR );
    HWND WINAPI createButton       ( LPCTSTR, HMENU );
    HWND WINAPI createTrackBar     ( HMENU );

    void WINAPI createModeControl ( );
    void WINAPI createSizeControl ( );
    void WINAPI createSimulationControl( );
    void WINAPI createEditorControl( );

    EvoController      * m_pEvoController;
    EvolutionModelData * m_pModelWork;
    INT                  m_iClientHeight;
    INT                  m_iBorder;
    INT                  m_iPosX;
    wstring              m_wstrGeneration;
    wstring              m_wstrScriptLine;

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
