// BlokusAppMenu.cpp
//
// Blokus

module BlokusAppMenu;

import Bitmap;
import BlokusCore;
import Debug;
import IconRedo;
import IconUndo;
import IoUtil;
import Resource;
import Win32_Util;
import Win32_Util_Resource;

using std::make_unique;

BlokusAppMenu::BlokusAppMenu()
  : m_upOnOffShowContactPnts(make_unique<OnOffPair>(IDD_CONTACT_PNTS)),
    m_upOnOffSound          (make_unique<OnOffPair>(IDD_SOUND           )),
    m_upOnOffShowCornerCells(make_unique<OnOffPair>(IDD_CORNER_CELLS)),
    m_upOnOffAnimation      (make_unique<OnOffPair>(IDD_ANIMATION))
{}

void BlokusAppMenu::Start(HWND const hwndApp)
{
    HINSTANCE const hInstance = GetModuleHandleW(nullptr);

	m_hwndApp = hwndApp;

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

	HBITMAP hBitmapUndo { CreateBitmapFromIconData(*IconUndo_data.data()) };
	HBITMAP hBitmapRedo { CreateBitmapFromIconData(*IconRedo_data.data()) };

    HMENU hMenuFile = ::PopupMenu(m_hMenu, L"&File");
    {
        ::AddMenu(hMenuFile, MF_STRING, IDD_RESET, L"&Reset");
        ::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,  L"&Exit");
    }

    ::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo);
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo);

    HMENU hMenuView = ::PopupMenu(m_hMenu, L"&View");
    {
        m_upOnOffShowContactPnts->AppendOnOffMenu(hMenuView, L"Contact points");
        m_upOnOffShowCornerCells->AppendOnOffMenu(hMenuView, L"Corner cells");
    }

    HMENU hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    {
        m_upOnOffAnimation->AppendOnOffMenu(hMenuOptions, L"&Animation");
         m_upOnOffSound   ->AppendOnOffMenu(hMenuOptions, L"&Sound");
   }
    HMENU hMenuHelp = ::PopupMenu(m_hMenu, L"&Help");
    {
  //      ::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        ::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    //DWORD err = GetLastError();
    //int x = 42;
    Assert(bRes);
}

void BlokusAppMenu::Notify(bool const bImmediately)
{
	m_upOnOffShowContactPnts->EnableOnOff(m_hMenu, BlokusPreferences::m_bShowContactPnts.Get());
	m_upOnOffShowCornerCells->EnableOnOff(m_hMenu, BlokusPreferences::m_bShowCornerCells.Get());
	m_upOnOffAnimation      ->EnableOnOff(m_hMenu, BlokusPreferences::m_bShowAnimation.Get());
    m_upOnOffSound          ->EnableOnOff(m_hMenu, Preferences::m_bSound.Get());
}
