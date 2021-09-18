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

    static void DoCall(WPARAM const wParam, LPARAM const lParam) 
    {
        AnimationCmd * const pAnimCmd       { reinterpret_cast<AnimationCmd * const>(lParam) };
        bool           const bTargetReached { static_cast<bool const>(wParam) };
        pAnimCmd->UpdateUI();
        if (bTargetReached && pAnimCmd->m_targetReachedFunc)
            (pAnimCmd->m_targetReachedFunc)(); 
    };

protected:
  
    void SetTargetReachedFunc(function<void()> const & func) { m_targetReachedFunc = func; }

    APP_PROC m_applicationFunc
    {
        [&](bool const bTargetReached)
        { 
            SendMessage
            (
                m_win.GetWindowHandle(), 
                WM_APP_UI_CALL, 
                static_cast<WPARAM>(bTargetReached), 
                reinterpret_cast<LPARAM>(this)
            );
        }
    };

private:
    MainWindow     & m_win;
    function<void()> m_targetReachedFunc { nullptr };
};