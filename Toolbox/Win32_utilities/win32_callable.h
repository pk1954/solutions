// win32_callable.h 
//
// Win32_utilities

#pragma once

#include <functional>
#include "windows.h"
#include "win32_util_resource.h"

using std::function;

class Callable
{
public:
    Callable(HWND const hwnd)
        : m_hwnd(hwnd)
    {}
    virtual ~Callable() {};
    virtual void Call_UI_thread(function<void()> const & func)
    {
        SendMessage(m_hwnd, WM_APP_UI_CALL, 0, reinterpret_cast<LPARAM>(&func));
    };

    static void DoCall(LPARAM const lParam) 
    {
        function<void()> const & func { * reinterpret_cast<function<void()> *>(lParam) };
        (func)();
    };

private:
    HWND m_hwnd { 0 };
};