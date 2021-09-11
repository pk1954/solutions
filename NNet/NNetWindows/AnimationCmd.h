// AnimationCmd.h
//
// NNetWindows

#pragma once

#include "win32_animation.h"
#include "win32_callable.h"
#include "win32_mainWindow.h"

using std::function;

class AnimationCmd
{
public:
    AnimationCmd(MainWindow & win)
        : m_win(win)
    {}

    virtual void DoAnimation  (function<void()> const &) = 0;
    virtual void UndoAnimation(function<void()> const &) = 0;

    virtual void UpdateUI() { m_win.Notify(false); };

protected:
  
    MainWindow               & m_win;
    function<void()>           m_targetReachedFunc { nullptr };
    function<void(bool const)> m_applicationFunc
    {
        [&](bool const bTargetReached)
        { 
            Callable callable { m_win.GetWindowHandle() };
            callable.Call_UI_thread([&](){ UpdateUI(); });
            if (bTargetReached && m_targetReachedFunc)
                callable.Call_UI_thread([&](){ (m_targetReachedFunc)(); });
        }
    };
};