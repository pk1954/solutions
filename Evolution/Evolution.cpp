// Evolution.cpp
//
// Evolution

#include "stdafx.h"
#include "Resource.h"
#include "win32_messagePump.h"
#include "win32_evoAppWindow.h"

int APIENTRY wWinMain
(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
)
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow );

	EvoAppWindow App;

	return MessagePump( hInstance, App, IDC_EVOLUTION_MAIN );
}

