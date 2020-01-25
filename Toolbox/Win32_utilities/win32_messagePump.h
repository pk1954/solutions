// win32_messagePump.h
//
// win32_utilities

#pragma once

#include "minwindef.h"

class AppWindowInterface;

int MessagePump
( 
	HINSTANCE const,
	HWND      const,
	int       const
);
