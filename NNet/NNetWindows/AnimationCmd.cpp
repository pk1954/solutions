// AnimationCmd.cpp
//
// NNetWindows

#include "stdafx.h"
#include "AnimationCmd.h"

AnimationCmd::AnimationCmd(MainWindow & win)
    : m_win(win)
{
    m_applicationFunc =
    [&](bool const bTargetReached)
    { 
        SendMessage
        (
            m_win.GetWindowHandle(), 
            WM_APP_UI_CALL, 
            static_cast<WPARAM>(bTargetReached), 
            reinterpret_cast<LPARAM>(this)
        );
    };
}

void AnimationCmd::DoCall(WPARAM const wParam, LPARAM const lParam) 
{
    AnimationCmd * const pAnimCmd       { reinterpret_cast<AnimationCmd * const>(lParam) };
    bool           const bTargetReached { static_cast<bool const>(wParam) };
    pAnimCmd->UpdateUI();
    if (bTargetReached && pAnimCmd->m_targetReachedFunc)
        (pAnimCmd->m_targetReachedFunc)(); 
};
