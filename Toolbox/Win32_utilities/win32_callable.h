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
    virtual ~Callable() {};
    virtual void Call_UI_thread(function<void()> const & func)
    {
        (func)();
    };

    static void DoCall(LPARAM const lParam) 
    {
        (*(reinterpret_cast<function<void()> *>(lParam)))();
    };
};