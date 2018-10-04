// win32_thread.h : 
//

#pragma once

#include "windows.h"

namespace Util
{
	HANDLE MakeThread( LPTHREAD_START_ROUTINE, LPVOID, LPDWORD );
};
