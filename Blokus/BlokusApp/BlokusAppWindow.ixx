// BlokusAppWindow.ixx
//
// Blokus

export module BlokusAppWindow;

import std;
import AboutBox;
import BaseWindow;
import BlokusAppMenu;
import BlokusCore;
import BlokusWindow;
import Commands;
import HiResTimer;
import MessagePump;
import Observable;
import ScriptHook;
import StatusBar;
import StatusBarDisplayFunctor;
import TournamentWindow;
import UndoRedoMenu;
import Win32_Sound;
import WinBasics;
import WinCommand;

using std::unique_ptr;;
using std::wstring;
using std::wcout;
using std::endl;

export class BlokusAppWindow : public BaseWindow
{
public:
	BlokusAppWindow(wstring const&, MessagePump&);

	void DoGameStuff() 
	{
		if (m_tournament.IsActive())
		{
			m_timer.BeforeAction();
			m_tournament.NextTournamentMove();
		 	m_timer.AfterAction();
			//wcout << L"DoGameStuff " << m_timer.Average2wstring() << endl;
		}
	}

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
	BlokusAppMenu           m_appMenu;
	BlokusWindow            m_mainWindow;
	CommandStack            m_cmdStack;
    HiResTimer              m_timer;
	MatchWriterInterface    m_mwi;
	Observable              m_matchObservable;
	ScriptHook              m_ScriptHook;
	StatusBar               m_statusBar;
	StatusBarDisplayFunctor m_statusBarDispFunctor;
	Tournament              m_tournament;
	TournamentWindow        m_tournamentWindow;
	UndoRedoMenu            m_undoRedoMenu;
	WinSound                m_sound;
	unique_ptr<Match>       m_upMatch;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
