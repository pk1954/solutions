// PlugIoNeuronAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Nob.h"
#include "Neuron.h"
#include "IoNeuron.h"
#include "SingleNobAnimation.h"
#include "win32_animationCmd.h"
#include "PlugIoNeurons.h"

class RootWindow;
class NNetModelWriterInterface;

class PlugIoNeuronAnimation : public AnimationCmd
{
public:
    PlugIoNeuronAnimation
    (
        NNetModelWriterInterface & nmwi,
        NobId                      idAnimated,
        NobId                      idTarget
    )
      : m_nobAnimated( * nmwi.GetNobPtr<IoNeuron *>(idAnimated) ),
        m_nobTarget  ( * nmwi.GetNobPtr<IoNeuron *>(idTarget) )
    {
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 3.0_MicroMeter)));
        AddPhase(make_unique<SingleNobAnimation>(m_nobAnimated, CalcOffsetPosDir(m_nobTarget, 1.4_MicroMeter)));
        AddPhase(make_unique<PlugIoNeurons     >(m_nobAnimated, m_nobTarget));
    }

    virtual ~PlugIoNeuronAnimation() {};

    virtual void Do()
    {
        m_nobAnimated.LockDirection(); 
        AnimationCmd::Do();
    }

    virtual void Undo()
    {
        AnimationCmd::Undo();
        m_nobAnimated.UnlockDirection();
    }

private:
    IoNeuron & m_nobAnimated;
    IoNeuron & m_nobTarget; 
};