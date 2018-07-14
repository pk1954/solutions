// win32_status.h 
//

#pragma once

#include <string>
#include "win32_baseDialog.h"
#include "gridPoint.h"

using namespace std;

class StatusBar : public BaseDialog
{
public:
    StatusBar( );

    void Start( HWND const);
	void SetSimuMode( BOOL const );

    int  GetHeight( ) const;
    void Resize( ) const;
    void SetSizeTrackBar ( short const ) const;
    void SetSpeedTrackBar( DWORD const ) const;
    void ClearStatusLine( );
    void DisplayStatusLine( std::wstring const & );
    void DisplayScriptLine( std::wstring const &, int, std::wstring const & );
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

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
