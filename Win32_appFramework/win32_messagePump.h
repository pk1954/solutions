// win32_messagePump.h
//
// Win32_appFramework

#pragma once

#include "minwindef.h"

class AppWindowInterface;

int MessagePump
( 
	HINSTANCE const,
	AppWindowInterface &,
	int const
);
