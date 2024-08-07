// Win32_MessagePump.cpp
//
// win32_utilities

module;

#include <Windows.h>

module MessagePump;

import std;
import Win32_Util;

void MessagePump::SetAccelTable(HACCEL const haccel)
{
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
			SetLastError(0);
			if (TranslateAccelerator(entry.m_hwnd, m_defaultAccelTable, &msg))
				return true;
			DWORD err = GetLastError();
			if (err)
			{
				int x = 42;
			}
			if (entry.m_bIsDialog && IsDialogMessage(entry.m_hwnd, &msg))
				return true;
		}
	}
	return false;
}
