// win32_animationSequence.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_rootWindow.h"
#include "win32_animationSequence.h"

AnimationSequence::AnimationSequence(RootWindow & win)
  : m_win(win)
{}

void AnimationSequence::Do()
{
    m_uiPhase = 0;
    DoPhase();
}

void AnimationSequence::Undo()
{
    m_uiPhase = Cast2Int(m_phases.size());
    UndoPhase();
}

void AnimationSequence::AddPhase(unique_ptr<AnimationCmd> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void AnimationSequence::BlockUI()   
{ 
    m_win.SendCommand2Application(IDM_BLOCK_UI, true);  
};

void AnimationSequence::UnblockUI()
{ 
    m_win.SendCommand2Application(IDM_BLOCK_UI, false); 
};

void AnimationSequence::DoPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase == 0)
        BlockUI();
    if (m_uiPhase < m_phases.size())
    {
        AnimationCmd & animCmd { * m_phases[m_uiPhase++] };
        animCmd.SetTargetReachedFunc( [&](){ DoPhase(); } );
        animCmd.DoAnimation();
    }
    else
        UnblockUI();
    m_win.Notify(false);
}

void AnimationSequence::UndoPhase() // runs in UI thread
{
    //wcout << L'#' << __FUNCDNAME__ << L" phase(" << m_uiPhase << L'/' << m_phases.size() << L')' << endl;
    if (m_uiPhase >= m_phases.size())
        BlockUI();
    if (m_uiPhase > 0)
    {
        AnimationCmd & animCmd { * m_phases[--m_uiPhase] };
        animCmd.SetTargetReachedFunc( [&](){ UndoPhase(); } );
        animCmd.UndoAnimation();
    }
    else
        UnblockUI();
    m_win.Notify(false);
}
