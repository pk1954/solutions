// win32_messagePump.h
//
// Win32_appFramework

module;

#include "minwindef.h"

class AppWindowInterface;

int MessagePump
(
	HINSTANCE const,
	AppWindowInterface &,
	int const
);
