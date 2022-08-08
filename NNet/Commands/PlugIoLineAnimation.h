// PlugIoLineAnimation.h
//
// Commands

#pragma once

#include "Nob.h"
#include "Neuron.h"
#include "IoLine.h"
#include "SingleNobAnimation.h"
#include "NNetCommand.h"
#include "PlugIoLines.h"

import MoreTypes;

class RootWindow;
class NNetModelWriterInterface;

class PlugIoLineAnimation : public NNetCommand
{
public:
    PlugIoLineAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
      : m_nobAnimated( * m_pNMWI->GetNobPtr<IoLine *>(idAnimated) ),
        m_nobTarget  ( * m_pNMWI->GetNobPtr<IoLine *>(idTarget) )
    {
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoLines       >(m_nobAnimated, m_nobTarget));
    }

    ~PlugIoLineAnimation() final = default;

    void Do() final
    {
        m_nobAnimated.LockDirection(); 
        Command::Do();
    }

    void Undo() final
    {
        Command::Undo();
        m_nobAnimated.UnlockDirection();
    }

    bool IsAsyncCommand() final
    { 
        return true; 
    };

private:
    IoLine & m_nobAnimated;
    IoLine & m_nobTarget; 
};