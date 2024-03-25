// Win32_MessagePump.ixx
//
// win32_utilities

module;

#include "Windows.h"
#include <vector>

export module MessagePump;

using std::vector;

export class MessagePump
{
public:
	void SetAccelTable(HACCEL const);
	void RegisterWindow(HWND const, bool const);

int Run(auto const gameFunc)
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (!accelerator(msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					break;
			}
		}
		else
		{
			gameFunc();
		}
	}

	return (int) msg.wParam;
}

private:
	struct AccEntry
	{
		HWND m_hwnd;
		bool m_bIsDialog;
	};

	vector<AccEntry> m_accEntries;
	HACCEL           m_defaultAccelTable { nullptr };

	bool accelerator(MSG &);
};