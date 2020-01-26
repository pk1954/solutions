// win32_resetDialog.h 
//
// Evolution

#pragma once

#include "Windows.h"

class ResetDialog
{
public:
	static int Show( HWND const );

	static int GetNewWidth();
	static int GetNewHeight();
	static int GetNewNrOfNeighbors();

private:
	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
