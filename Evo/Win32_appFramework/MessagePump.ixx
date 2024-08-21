// MessagePump.ixx
//
// Win32_appFramework

export module MessagePump;

import WinBasics;
import AppWindowInterface;

int MessagePump
(
	HINSTANCE const,
	AppWindowInterface &,
	int const
);
