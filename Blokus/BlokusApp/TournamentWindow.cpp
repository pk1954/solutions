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
import SaveCast;
import WinBasics;
import Win32_Controls;
import BlokusCore;
import Resource;

using std::wcout;
using std::endl;
using std::wstring;
using std::make_unique;

TournamentWindow::TournamentWindow(HWND const hwndParent)
{
    m_strategyNameList.push_back(&StrategyRandom        ::NAME);
    m_strategyNameList.push_back(&StrategyTakeFirst     ::NAME);
    m_strategyNameList.push_back(&StrategyBigFirstRandom::NAME);

    HWND hwnd = StartBaseWindow
	(
		hwndParent,
		L"ClassTournamentWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CAPTION|WS_SIZEBOX,
		nullptr,
		nullptr
	);
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

    m_upTournament = make_unique<Tournament>();
    m_table.Initialize(hwnd, m_upTournament.get());
}

void TournamentWindow::DoGameStuff()
{ 
    if (m_upTournament)
    {
		m_upTournament->DoGameStuff(); 
        m_table.Notify(false);
    }
}

bool TournamentWindow::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	WORD const wId    { LoWord(wParam) }; 
    WORD const wEvent { HiWord(wParam) };
    if (isComboBoxId(wId))
    {
        if (wEvent == CBN_SELCHANGE)
        {
            PlayerId const idPlayer  { Cast2SignedChar(wId - ID_FIRST_COMBO_BOX) };
            HWND     const hComboBox { (HWND)lParam };
            int      const iStratNr  { Cast2Int(GetCurSel(hComboBox)) };
            m_upTournament->SetStrategy(idPlayer, *m_strategyNameList.at(iStratNr));
        }
    }
    else if (wId == IDD_START_TOURNAMENT)
    {
        m_upTournament->Start(100, 0);
    }
    else if (wId == IDD_STOP_TOURNAMENT)
    {
        m_upTournament->Stop();
    }
    return true;

	//return GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
}

bool TournamentWindow::OnSize(PIXEL const width, PIXEL const height)
{
    m_table.Move(0_PIXEL, 0_PIXEL, width, 150_PIXEL, true);
    m_table.Trigger(false);
    return true;
}
