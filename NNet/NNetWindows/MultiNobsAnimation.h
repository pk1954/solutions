// MultiNobsAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_mainWindow.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "IoNeuronList.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "AnimationCmd.h"

using std::function;

class MultiNobsAnimation : public AnimationCmd
{
public:
    MultiNobsAnimation
    (
        MainWindow                & win,
        IoNeuronList              & nobs,
        MicroMeterPntVector const & umPntVectorTarget
    )
      : AnimationCmd(win),
        m_nobsAnimated(nobs),
        m_umPntVectorStart(nobs),
        m_umPntVectorTarget(umPntVectorTarget)
    {}

    virtual void Do(function<void()> const & func)
    {
        m_targetReachedFunc = func;
        MicroMeterPntVector const umPntVectorActual(m_nobsAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPntVectorActual, m_umPntVectorTarget));
        m_animation.Start(umPntVectorActual, m_umPntVectorTarget);
    }

    virtual void Undo(function<void()> const & func)
    {
        m_targetReachedFunc = func;
        MicroMeterPntVector const umPntVectorActual(m_nobsAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPntVectorActual, m_umPntVectorStart));
        m_animation.Start(umPntVectorActual, m_umPntVectorStart);
    }

    virtual void UpdateUI()
    {
        for (size_t i = 0; i < m_nobsAnimated.Size(); ++i)
            m_nobsAnimated.GetElem(i).SetPosDir(m_animation.GetActual().GetPosDir(i));
        AnimationCmd::UpdateUI();
    }

private:

    IoNeuronList              m_nobsAnimated;
    MicroMeterPntVector const m_umPntVectorStart;
    MicroMeterPntVector const m_umPntVectorTarget;

    Animation<MicroMeterPntVector> m_animation 
    {
        [&](bool const bTargetReached)
        { 
            Callable callable { m_win.GetWindowHandle() };
            callable.Call_UI_thread([&](){ UpdateUI(); });
            if (bTargetReached && m_targetReachedFunc)
                callable.Call_UI_thread([&](){ (m_targetReachedFunc)(); });
        }
    };
};