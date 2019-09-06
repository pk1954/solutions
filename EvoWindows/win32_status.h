// win32_status.h 
//

#pragma once

#include <string>
#include "PixelTypes.h"
#include "EvolutionTypes.h"
#include "EvoReadBuffer.h"
#include "win32_rootWindow.h"

class Delay;
class EditorWindow;
class HistorySystem;
class EvoWorkThreadInterface;

using std::wstring;

class StatusBar : public RootWindow
{
public:
	StatusBar();

	void  Start
	( 
		HWND                           const, 
		EvoReadBuffer                * const, 
		HistorySystem          const * const, 
		EvoWorkThreadInterface const * const, 
		Delay                        * const, 
		EditorWindow                 * const 
	);
	void  Stop( );

	PIXEL GetHeight( ) const;
    void  Resize( ) const;
    void  SetSizeTrackBar ( PIXEL const ) const;
    void  SetSpeedTrackBar( DWORD const ) const;
    void  ClearStatusLine( );
    void  DisplayStatusLine( std::wstring const & );
    void  DisplayScriptLine( std::wstring const &, int, std::wstring const & );
    void  DisplayCurrentGeneration( EVO_GENERATION const );

private:

    enum class tPart
    {
        Generation,
		Mode,
        Size,
        SimuCtl,
        ScriptLine,
        Stop
    };

	void adjust( );
	void maxSpeed( );
	void scrollBarMessage( int const );

	HWND WINAPI createControl      ( LPCTSTR, LPCTSTR, DWORD, HMENU );
    HWND WINAPI createStaticControl( LPCTSTR );
    HWND WINAPI createButton       ( LPCTSTR const, HMENU const, DWORD const  );
    HWND WINAPI createTrackBar     ( HMENU );

	void WINAPI createSizeControl ( );
    void WINAPI createSimulationControl( );

	PIXEL m_pixClientHeight;
    PIXEL m_pixBorderX;
    PIXEL m_pixBorderY;
    PIXEL m_pixPosX;
    wstring m_wstrGeneration;
    wstring m_wstrScriptLine;

	Delay                        * m_pDelay;
	EvoReadBuffer                * m_pReadBuffer;
	EditorWindow                 * m_pEditorWindow;
	HistorySystem          const * m_pHistorySystem;
	EvoWorkThreadInterface const * m_pWorkThreadInterface;

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

friend static LRESULT CALLBACK OwnerDrawStatusBar( HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR );
};
