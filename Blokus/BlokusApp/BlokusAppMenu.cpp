// BlokusAppMenu.cpp
//
// BlokusApp

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

void BlokusAppMenu::Initialize(HWND const hwndApp)
{
	m_hwndApp = hwndApp;
    m_hMenu   = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

    m_hMenuFile    = ::PopupMenu(m_hMenu, L"&File");
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)CreateBitmapFromIconData(*IconUndo_data.data()));
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)CreateBitmapFromIconData(*IconRedo_data.data()));
    m_hMenuView    = ::PopupMenu(m_hMenu, L"&View");
    m_hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    m_hMenuHelp    = ::PopupMenu(m_hMenu, L"&Help");

    bool bRes = SetMenu(m_hwndApp, m_hMenu); Assert(bRes);

    ::AddMenu(m_hMenuFile, MF_STRING, IDD_RESET, L"&Reset");
    ::AddMenu(m_hMenuFile, MF_STRING, IDM_EXIT,  L"&Exit");
    ::AddMenu(m_hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
}

void BlokusAppMenu::Notify(bool const bImmediately)
{
	BoolPreferences::EnableOnOff(m_hMenu);
}
