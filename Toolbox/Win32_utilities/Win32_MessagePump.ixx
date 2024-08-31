// Win32_MessagePump.ixx
//
// win32_utilities

export module MessagePump;

import std;
import WinBasics;
import Win32_Util_Resource;
import WinCommand;

using std::vector;
using std::runtime_error;

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
			if (PeekMessageW(&msg, (HWND)0, 0, 0, PM_REMOVE))
			{
				if (!accelerator(msg))
				{
					TranslateMessage(&msg);
					dispatch(msg);
					if (msg.message == WM_APP_UI_CALL)
						WinCommand::AnimationUpdate(msg.wParam, msg.lParam);
					else if (msg.message == WM_QUIT)
						break;
				}
			}
			__try 
			{
				gameFunc();
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				throw runtime_error("Access violation in gameFunc");
			}
		}

		return (int)msg.wParam;
	}

	int Run()
	{
		MSG msg;

		while (GetMessageW(&msg, HWND(0), 0, 0) != 0) 
		{
			TranslateMessage(&msg); // Translate virtual-key messages
			dispatch(msg);         // Dispatch the message to the appropriate window procedure
		}
		return (int)msg.wParam;
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
	void dispatch(MSG const&);
};