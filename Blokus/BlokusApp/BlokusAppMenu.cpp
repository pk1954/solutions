// BlokusAppMenu.cpp
//
// Blokus

module BlokusAppMenu;

import Bitmap;
import BoolPreferences;
import BlokusCore;
import Debug;
import IconRedo;
import IconUndo;
import IoUtil;
import Resource;
import Win32_Util;
import Win32_Util_Resource;

using std::make_unique;

void BlokusAppMenu::Start(HWND const hwndApp)
{
    HINSTANCE const hInstance = GetModuleHandleW(nullptr);

	m_hwndApp = hwndApp;

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

	//HBITMAP hBitmapUndo { CreateBitmapFromIconData(*IconUndo_data.data()) };
	//HBITMAP hBitmapRedo { CreateBitmapFromIconData(*IconRedo_data.data()) };

    HMENU hMenuFile    = ::PopupMenu(m_hMenu, L"&File");
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)CreateBitmapFromIconData(*IconUndo_data.data()));
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)CreateBitmapFromIconData(*IconRedo_data.data()));
    HMENU hMenuView    = ::PopupMenu(m_hMenu, L"&View");
    HMENU hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    HMENU hMenuHelp    = ::PopupMenu(m_hMenu, L"&Help");

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    //DWORD err = GetLastError();
    //int x = 42;
    Assert(bRes);

    {
        ::AddMenu(hMenuFile, MF_STRING, IDD_RESET, L"&Reset");
        ::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,  L"&Exit");
    }
    {
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowPieceNumbers, hMenuView, L"Piece numbers");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowContactPnts,  hMenuView, L"Contact points");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowCellNumbers,  hMenuView, L"Cell numbers");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowBlockedCells, hMenuView, L"Blocked cells");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowCornerCells,  hMenuView, L"Corner cells");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowMoveDetail,   hMenuView, L"Move details");
    }
    {
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefShowAnimation, hMenuOptions, L"&Animation");
        BoolPreferences::AppendOnOffMenu(BlokusPreferences::m_bPrefSound,         hMenuOptions, L"&Sound");
    }
    {
  //      ::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        ::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }
}

void BlokusAppMenu::Notify(bool const bImmediately)
{
	BoolPreferences::EnableOnOff(m_hMenu);
}
