// win32_thread.cpp :
//

#include "stdafx.h"
#include "win32_thread.h"

HANDLE Util::MakeThread
( 
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID   lpParameter,
    LPDWORD  lpThreadId
)
{
    //lint -esym( 954, hndThread )     HANDLE could be pointer to const
    HANDLE const hThread = CreateThread( nullptr, 0, lpStartAddress, lpParameter, 0, lpThreadId );

    assert( hThread != nullptr );

    return hThread;
}  
