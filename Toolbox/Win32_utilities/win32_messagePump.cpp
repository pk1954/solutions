// Win32_MessagePump.cpp
//
// win32_utilities

module;

#include <cassert>
#include <array>
#include "Windows.h"

#include <iostream>

module MessagePump;

import Win32_Util;

//void MessagePump::SetAccelTable(HACCEL const haccel)
//{
//	m_defaultAccelTable = haccel;
//}

void MessagePump::SetAccelTable(HINSTANCE const hInstance, int const idResource)
{
	HACCEL haccel{ LoadAccelerators(hInstance, MAKEINTRESOURCE(idResource)) };
	m_defaultAccelTable = haccel;
}

void MessagePump::RegisterWindow(HWND const hwnd, bool const bIsDialog)
{
	m_accEntries.push_back(AccEntry{ hwnd, bIsDialog });
}

bool MessagePump::accelerator(MSG& msg)
{
	for (AccEntry const& entry : m_accEntries)
	{
		if ((entry.m_hwnd == msg.hwnd) || IsChild(entry.m_hwnd, msg.hwnd))
		{
			if (TranslateAccelerator(entry.m_hwnd, m_defaultAccelTable, &msg))
				return true;
			if (entry.m_bIsDialog && IsDialogMessage(entry.m_hwnd, &msg))
				return true;
		}
	}
	return false;
}

//bool MessagePump::accelerator(MSG& msg)
//{
//	if (msg.message == WM_KEYDOWN)
//	{
//		std::wcout << std::hex << msg.hwnd << L' ' << msg.message << L' ' << msg.wParam << L' ' << msg.lParam << std::endl;
//		HWND hwnd = msg.hwnd;
//		int i = 0;
//		while (hwnd)
//		{
//			std::wcout << ++i << L' ' << GetClassName(hwnd) << std::endl;
//			hwnd = GetParent(hwnd);
//		}
//	}
//
//	for (AccEntry const& entry : m_accEntries)
//	{
//		bool bTranslate = false;
//		if (msg.message == WM_KEYDOWN)
//		{
//			std::wcout << L"Testing " << GetClassName(entry.m_hwnd) << std::endl;
//			if (entry.m_hwnd == msg.hwnd)
//			{
//				bTranslate = true;
//				std::wcout << L"identical" << std::endl;
//			}
//			else if (IsChild(entry.m_hwnd, msg.hwnd))
//			{
//				bTranslate = true;
//				std::wcout << L"child" << std::endl;
//			}
//			else
//				std::wcout << L"no fit" << std::endl;
//		}
//		if (bTranslate)
//		{
//			if (TranslateAccelerator(entry.m_hwnd, m_defaultAccelTable, &msg))
//			{
//				if (msg.message == WM_KEYDOWN)
//					std::wcout << L"TranslateAccelerator: true " << std::endl;
//				return true;
//			}
//			else
//				if (msg.message == WM_KEYDOWN)
//					std::wcout << L"TranslateAccelerator: false " << std::endl;
//			if (entry.m_bIsDialog && IsDialogMessage(entry.m_hwnd, &msg))
//				return true;
//		}
//	}
//	return false;
//}
//
int MessagePump::Run()
{
	MSG  msg;
	bool bRet;

	while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0)    // Main message loop //-V559 //-V593
	{
		assert(bRet != -1); //-V547
		if (! accelerator(msg))
		{
			(void)TranslateMessage(&msg);
			(void)DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
