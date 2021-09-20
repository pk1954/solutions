// win32_animationCmd.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_rootWindow.h"
#include "win32_animationCmd.h"

AnimationCmd::AnimationCmd()
{
    m_applicationFunc =
    [&](bool const bTargetReached)
    { 
        SendMessage
        (
            m_pWin->GetWindowHandle(), 
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

void AnimationCmd::Do()
{
    m_uiPhase = 0;
    DoPhase();
}

void AnimationCmd::Undo()
{
    m_uiPhase = Cast2Int(m_phases.size());
    UndoPhase();
}

void AnimationCmd::AddPhase(unique_ptr<AnimationCmd> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void AnimationCmd::BlockUI()   
{ 
    m_pWin->SendCommand2Application(IDM_BLOCK_UI, true);  
};

void AnimationCmd::UnblockUI()
{ 
    m_pWin->SendCommand2Application(IDM_BLOCK_UI, false); 
};

void AnimationCmd::DoPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase == 0)
        BlockUI();
    if (m_uiPhase < m_phases.size())
    {
        AnimationCmd & animCmd { * m_phases[m_uiPhase++] };
        animCmd.SetTargetReachedFunc( [&](){ DoPhase(); } );
        animCmd.Do();
    }
    else
        UnblockUI();
    UpdateUI();
}

void AnimationCmd::UndoPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase >= m_phases.size())
        BlockUI();
    if (m_uiPhase > 0)
    {
        AnimationCmd & animCmd { * m_phases[--m_uiPhase] };
        animCmd.SetTargetReachedFunc( [&](){ UndoPhase(); } );
        animCmd.Undo();
    }
    else
        UnblockUI();
    UpdateUI();
}
