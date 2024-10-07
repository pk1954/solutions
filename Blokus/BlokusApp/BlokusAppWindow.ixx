// BlokusAppWindow.ixx
//
// Blokus

export module BlokusAppWindow;

import std;
import WinBasics;
import BaseWindow;
import AboutBox;
import MessagePump;
import StatusBar;
import ScriptHook;
import StatusBarDisplayFunctor;
import BlokusCore;
import BlokusAppMenu;
import BlokusWindow;
import TournamentWindow;

using std::wstring;

export class BlokusAppWindow : public BaseWindow
{
public:
	BlokusAppWindow(wstring const&, MessagePump&);

	void DoGameStuff() { }

	BlokusAppWindow            (BlokusAppWindow const&) = delete;  // noncopyable class 
	BlokusAppWindow& operator= (BlokusAppWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void configureStatusBar();
	
	int             m_statusMessagePart { };
	HWND            m_hwndApp           { nullptr };
	wstring const * m_pwstrProductName  { nullptr };

	AboutBox                m_aboutBox;
	StatusBar               m_statusBar;
	BlokusWindow            m_mainWindow;
	ScriptHook              m_ScriptHook;
	BlokusAppMenu           m_appMenu;
	Tournament              m_tournament;
	TournamentWindow        m_tournamentWindow;
	StatusBarDisplayFunctor m_statusBarDispFunctor;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
