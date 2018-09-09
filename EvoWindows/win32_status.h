// win32_status.h 
//

#pragma once

#include <string>
#include "win32_baseDialog.h"

using namespace std;

class EvolutionCore;

class StatusBar : public BaseDialog
{
public:
    StatusBar( );

    void Start( HWND const, EvolutionCore const * );
	void SetSimuMode( BOOL const );

    int  GetHeight( ) const;
    void Resize( ) const;
    void SetSizeTrackBar ( short const ) const;
    void SetSpeedTrackBar( DWORD const ) const;
    void ClearStatusLine( );
    void DisplayStatusLine( wstring const & );
    void DisplayScriptLine( wstring const &, int, wstring const & );
    void DisplayCurrentGeneration( EVO_GENERATION const );

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

    HWND WINAPI createControl      ( LPCTSTR, LPCTSTR, DWORD, HMENU );
    HWND WINAPI createStaticControl( LPCTSTR );
    HWND WINAPI createButton       ( LPCTSTR, HMENU );
    HWND WINAPI createTrackBar     ( HMENU );

    void WINAPI createModeControl ( );
    void WINAPI createSizeControl ( );
    void WINAPI createSimulationControl( );
    void WINAPI createEditorControl( );

    INT     m_iClientHeight;
    INT     m_iBorder;
    INT     m_iPosX;
    wstring m_wstrGeneration;
    wstring m_wstrScriptLine;

	EvolutionCore const * m_pCore;

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
