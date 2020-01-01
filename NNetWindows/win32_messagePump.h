// win32_messagePump.h
//
// NNetWindows

#pragma once

#include "minwindef.h"

class AppWindowInterface;

int MessagePump
( 
	HINSTANCE const,
	AppWindowInterface &,
	int const
);
