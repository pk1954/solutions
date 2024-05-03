// NNetViewerMenu.cpp
//
// NNetViewer

module;

#include <cassert>
#include <memory>
#include <Windows.h>
#include "Resource.h"

module NNetViewerMenu;

import Win32_Util;

using std::make_unique;

NNetViewerMenu::NNetViewerMenu()
{ }

NNetViewerMenu::~NNetViewerMenu() = default;

void NNetViewerMenu::Start(HWND const hwndScanViewerWindow) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_hwndScanViewerWindow = hwndScanViewerWindow;

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

    HMENU hMenuFile = ::PopupMenu(m_hMenu, L"&File");
    {
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
    }

    bool bRes = SetMenu(hwndScanViewerWindow, m_hMenu);
    assert(bRes);
}

void NNetViewerMenu::Notify(bool const bImmediately)
{
}
