// BaseCommand.cpp
//
// Toolbox\Commands

module;

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

module BaseCommand;

import SaveCast;
import SoundInterface;

using std::unique_ptr;

BaseCommand::BaseCommand()
{
    int x = 42;
}

void BaseCommand::Initialize(Sound* const pSound)
{
    m_pSound = pSound;
}

void BaseCommand::TargetReached()
{
    if (m_targetReachedFunc)
        (m_targetReachedFunc)(this);
}

void BaseCommand::Do()
{
    m_uiPhase = 0;
    doPhase();
}

void BaseCommand::Undo()
{
    m_uiPhase = Cast2UnsignedInt(m_phases.size());
    undoPhase();
}

void BaseCommand::AddPhase(unique_ptr<BaseCommand> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void BaseCommand::doPhase() // runs in UI thread
{
    if (!m_phases.empty())
    {
        if (m_uiPhase == 0)
            BlockUI();
        if (m_uiPhase < m_phases.size())
        {
            BaseCommand* const pAnimCmd { m_phases[m_uiPhase++].get() };
            pAnimCmd->m_targetReachedFunc = &BaseCommand::doPhase;
            pAnimCmd->Do();
        }
        else
            UnblockUI();
    }
    UpdateUI();
}

void BaseCommand::undoPhase() // runs in UI thread
{
    if (m_uiPhase >= m_phases.size())
        BlockUI();
    if (m_uiPhase > 0)
    {
        BaseCommand& animCmd { *m_phases[--m_uiPhase] };
        animCmd.m_targetReachedFunc = &BaseCommand::undoPhase;
        animCmd.Undo();
    }
    else
        UnblockUI();
    UpdateUI();
}

