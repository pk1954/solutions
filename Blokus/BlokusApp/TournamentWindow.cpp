// TournamentWindow.cpp
//
// BlokusApp
// 
// Handles Tournament UI:
// - Results life view
// - Select player strategies
// - Start/Stop buttons
// interfaces to Tournament

module TournamentWindow;

import std;
import Types;
import SaveCast;
import WinBasics;
import Win32_Controls;
import BlokusCore;
import Resource;

using std::wcout;
using std::endl;
using std::wstring;
using std::to_wstring;
using std::make_unique;

TournamentWindow::TournamentWindow(HWND const hwndApp)
{
    m_strategyNameList.push_back(&StrategyRandom        ::NAME);
    m_strategyNameList.push_back(&StrategyTakeFirst     ::NAME);
    m_strategyNameList.push_back(&StrategyBigFirstRandom::NAME);
    m_upTournament = make_unique<Tournament>();
	Initialize(hwndApp, L"ClassTournamentWindow", WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CAPTION| WS_SIZEBOX);

    for (int i = 0; i < NR_OF_PLAYERS; ++i) 
    { 
        m_comboBox[i] = CreateDropDownList
        (
	        GetWindowHandle(), 
            60, 30 + (i * 30), 90, 100,
	        ((UINT_PTR)(ID_FIRST_COMBO_BOX + i)), 
	        0
        );
        for (auto it : m_strategyNameList)
            AddString(m_comboBox[i], *it);
        SetCurSel(m_comboBox[i], 0);
    }

    HWND hwndStartButton = CreateButton(GetWindowHandle(), L"Start",  20, 160, 80, 20, IDD_START_TOURNAMENT);
    HWND hwndStopButton  = CreateButton(GetWindowHandle(), L"Stop",  150, 160, 80, 20, IDD_STOP_TOURNAMENT);

    TableWindow::Resize(RasterPoint(4,7));
    TableWindow::SetMinRowHeight(30._fPixel);
}

bool TournamentWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
    WORD const wNotificationCode { HiWord(wParam) };

    if (wNotificationCode == 0)  // message from menu
    { 
        return true;
    }
    else if (wNotificationCode == 1)  // accelerator
    { 
        return true;
    }
    else // message from control
    {
	    switch (WORD const wCtrlId { LoWord(wParam) } )
	    {
	    case CBN_SELCHANGE:
             {
                 PlayerId const idPlayer  { Cast2SignedChar(wCtrlId) };
                 HWND     const hComboBox { (HWND)lParam };
                 int      const iStratNr  { Cast2Int(GetCurSel(hComboBox)) };
                 m_upTournament->SetStrategy(idPlayer, *m_strategyNameList.at(iStratNr));
             }
             return true;

	    case IDD_START_TOURNAMENT:
             m_iNrOfMatches = 100;
             m_iNrOfThreads = 0;
             m_bActive = true;
             m_upTournament->Start(m_iNrOfMatches, m_iNrOfThreads);
             return true;

	    case IDD_STOP_TOURNAMENT:
             m_upTournament->Stop();
             return true;

	    default:
             return true;
	    }
    }

	return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
}

void TournamentWindow::PaintGraphics()
{
	m_timer.BeforeAction();
    Ticks ticksAllPlayers { 0 };
    TableWindow::Clear();
    Set(RasterPoint(0, 0), to_wstring(m_iMatch) +
                            L"/" +
                            to_wstring(m_iNrOfMatches));
    Set(RasterPoint(1, 0), L"Strategy");
    Set(RasterPoint(2, 0), L"#Wins");
    Set(RasterPoint(3, 0), L"Time");

    Apply2AllPlayerIds
    (
        [this, &ticksAllPlayers](PlayerId id)
        {
            Ticks const ticksPlayer { m_upTournament->GetTimeUsed(id) };
            int   const yPos        { id.GetValue() + 1 };
            Set(RasterPoint(0, yPos), Components::GetPlayerType(id).m_wstrName);
            Set(RasterPoint(1, yPos), m_upTournament->GetStrategyName(id));
            Set(RasterPoint(2, yPos), m_upTournament->NrOfWins(id));
            Set(RasterPoint(3, yPos), PerfCounter::Ticks2wstring(ticksPlayer));
            ticksAllPlayers += ticksPlayer;
        }
    );

    //Ticks   ticksTournament { m_pTournament->GetTournamentTime() };
    //Ticks   ticksOverhead   { ticksTournament - ticksAllPlayers };
    //PERCENT percentOverhead { CalcPercent(ticksOverhead, ticksTournament) };
    //Set(RasterPoint(2, 5), L"tournament");
    //Set(RasterPoint(3, 5), PerfCounter::Ticks2wstring(ticksTournament));
    //Set(RasterPoint(2, 6), L"overhead");
    //Set(RasterPoint(3, 6), PerfCounter::Ticks2wstring(ticksOverhead) + L" " + 
    //                       Percent2wstring(percentOverhead));

    TableWindow::CalcRowsAndCols();
    TableWindow::PaintGraphics();
 	m_timer.AfterAction();
    //wcout << L"Graphics " << m_timer.Average2wstring() << endl;
}
