// SingleNobAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_mainWindow.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "AnimationCmd.h"

using std::function;

class SingleNobAnimation : public AnimationCmd
{
public:
    SingleNobAnimation
    (
        MainWindow             & win,
        Nob                    & nob,
        MicroMeterPosDir const & umPosDirTarget
   )
      : AnimationCmd(win),
        m_nobAnimated(nob),
        m_umPosDirStart(nob.GetPosDir()),
        m_umPosDirTarget(umPosDirTarget)
    {}

    virtual void DoAnimation(function<void()> const & func)
    {
        wcout << L'#' << __FUNCDNAME__ << endl;
        m_targetReachedFunc = func;
        MicroMeterPosDir const umPosDirActual(m_nobAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPosDirActual, m_umPosDirTarget));
        m_animation.Start(umPosDirActual, m_umPosDirTarget);
    }

    virtual void UndoAnimation(function<void()> const & func)
    {
        wcout << L'#' << __FUNCDNAME__ << endl;
        m_targetReachedFunc = func;
        MicroMeterPosDir const umPosDirActual(m_nobAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPosDirActual, m_umPosDirStart));
        m_animation.Start(umPosDirActual, m_umPosDirStart);
    }

    virtual void UpdateUI()
    {
        m_nobAnimated.SetPosDir(m_animation.GetActual());
        AnimationCmd::UpdateUI();
    }

private:

    Nob                       & m_nobAnimated;
    MicroMeterPosDir      const m_umPosDirStart;
    MicroMeterPosDir      const m_umPosDirTarget;
    Animation<MicroMeterPosDir> m_animation  { m_applicationFunc };
};