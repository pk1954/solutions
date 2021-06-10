// AnimationSequence.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetModelWriterInterface.h"
#include "win32_mainWindow.h"
#include "SingleNobAnimation.h"
#include "AnimationSequence.h"

AnimationSequence::AnimationSequence(MainWindow & win)
  : m_win(win)
{}

void AnimationSequence::Do(NNetModelWriterInterface& nmwi)
{
    m_uiPhase = 0;
    doPhase();
}

void AnimationSequence::Undo(NNetModelWriterInterface& nmwi)
{
    m_uiPhase = Cast2Int(m_phases.size());
    undoPhase();
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

void AnimationSequence::doPhase() // runs in UI thread
{
    if (m_uiPhase == 0)
        BlockUI();
    if (m_uiPhase < m_phases.size())
         m_phases[m_uiPhase++]->Do([&](){ doPhase(); } );
    else
        UnblockUI();
    m_win.Notify(false);
}

void AnimationSequence::undoPhase() // runs in UI thread
{
    if (m_uiPhase >= m_phases.size())
        BlockUI();
    if (m_uiPhase > 0)
        m_phases[--m_uiPhase]->Undo([&](){ undoPhase(); });
    else
        UnblockUI();
    m_win.Notify(false);
}
