// PlugIoNeuronAnimation.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "SingleNobAnimation.h"
#include "NNetCommand.h"
#include "PlugIoNeurons.h"

class RootWindow;
class NNetModelWriterInterface;

class PlugIoNeuronAnimation : public NNetCommand
{
public:
    PlugIoNeuronAnimation
    (
        NobId idAnimated,
        NobId idTarget
    )
      : m_nobAnimated( * m_pNMWI->GetNobPtr<IoNeuron *>(idAnimated) ),
        m_nobTarget  ( * m_pNMWI->GetNobPtr<IoNeuron *>(idTarget) )
    {
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoNeurons     >(m_nobAnimated, m_nobTarget));
    }

    virtual ~PlugIoNeuronAnimation() {};

    virtual void Do()
    {
        m_nobAnimated.LockDirection(); 
        Command::Do();
    }

    virtual void Undo()
    {
        Command::Undo();
        m_nobAnimated.UnlockDirection();
    }

    virtual bool IsAsyncCommand() 
    { 
        return true; 
    };

private:
    IoNeuron & m_nobAnimated;
    IoNeuron & m_nobTarget; 
};