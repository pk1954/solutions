// IoNeuronsAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_mainWindow.h"
#include "win32_animation.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPntVector.h"
#include "NNetCommand.h"

using std::function;

class IoNeuronsAnimation : public NNetCommand
{
public:
    IoNeuronsAnimation
    (
        vector<IoNeuron *>        & nobs,
        MicroMeterPntVector const & umPntVectorTarget
   )
      : m_nobsAnimated(nobs),
        m_umPntVectorStart(nobs),
        m_umPntVectorTarget(umPntVectorTarget)
    {}

    virtual void DoAnimation(function<void()> const & targetReachedFunc)
    {
//        wcout << L'#' << __FUNCDNAME__ << endl;
        SetTargetReachedFunc(targetReachedFunc);
        MicroMeterPntVector const umPntVectorActual(m_nobsAnimated);
        for (auto & it: m_nobsAnimated)
            it->LockDirection(); 
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPntVectorActual, m_umPntVectorTarget));
        m_animation.Start(umPntVectorActual, m_umPntVectorTarget);
    }

    virtual void UndoAnimation(function<void()> const & targetReachedFunc)
    {
//        wcout << L'#' << __FUNCDNAME__ << endl;
        SetTargetReachedFunc(targetReachedFunc);
        MicroMeterPntVector const umPntVectorActual(m_nobsAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPntVectorActual, m_umPntVectorStart));
        m_animation.Start(umPntVectorActual, m_umPntVectorStart);
    }

    virtual void UpdateUI()
    {
        for (size_t i = 0; i < m_nobsAnimated.size(); ++i)
            m_nobsAnimated.at(i)->SetPosDir(m_animation.GetActual().GetPosDir(i));
        AnimationCmd::UpdateUI();
    }

private:

    vector<IoNeuron *>             m_nobsAnimated;
    MicroMeterPntVector      const m_umPntVectorStart;
    MicroMeterPntVector      const m_umPntVectorTarget;
    Animation<MicroMeterPntVector> m_animation { m_applicationFunc };
};