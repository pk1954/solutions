// AnimationCmd.h
//
// NNetWindows

#pragma once

#include "win32_animation.h"
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

    static void DoCall(LPARAM const lParam) 
    {
        function<void()> const & func { * reinterpret_cast<function<void()> *>(lParam) };
        (func)();
    };

protected:
  
    MainWindow      & m_win;
    function<void()> m_targetReachedFunc { nullptr };
    APP_PROC         m_applicationFunc
    {
        [&](bool const bTargetReached)
        { 
            function<void()> const & func
            {
                [&]()
                { 
                    UpdateUI();
                    if (bTargetReached && m_targetReachedFunc)
                        (m_targetReachedFunc)(); 
                }
            };

            SendMessage
            (
                m_win.GetWindowHandle(), 
                WM_APP_UI_CALL, 
                0, 
                reinterpret_cast<LPARAM>(&func)
            );
        }
    };
};