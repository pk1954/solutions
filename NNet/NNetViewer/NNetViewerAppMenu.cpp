// NNetViewerAppMenu.cpp
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <Windows.h>
#include "Resource.h"

module NNetViewerAppMenu;

import Win32_Util_Resource;
import SoundInterface;
import WinManager;
import BaseWindow;
import Win32_Util;
import OnOffPair;
import CommandStack;
import NNetModel;
import Preferences;
import NNetPreferences;

using std::make_unique;

NNetViewerAppMenu::NNetViewerAppMenu()
{ }

NNetViewerAppMenu::~NNetViewerAppMenu() = default;

void NNetViewerAppMenu::Start(HWND const hwndApp) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_hwndApp = hwndApp;

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

    HMENU hMenuFile = ::PopupMenu(m_hMenu, L"&File");
    {
        ::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit");
    }

    HMENU hMenuAction = ::PopupMenu(m_hMenu, L"Action");
    {
    }

    HMENU hMenuView = ::PopupMenu(m_hMenu, L"&View");
    {
    }
    HMENU hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    {
    }
    HMENU hMenuHelp = ::PopupMenu(m_hMenu, L"&Help");
    {
        ::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        ::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    assert(bRes);
}

void NNetViewerAppMenu::Notify(bool const bImmediately)
{
    DrawMenuBar(m_hwndApp);
}
