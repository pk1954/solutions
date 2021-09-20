// SingleNobAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_mainWindow.h"
#include "win32_animation.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "NNetCommand.h"

using std::function;

class SingleNobAnimation : public NNetCommand
{
public:
    SingleNobAnimation
    (
        Nob                    & nobAnimated,
        MicroMeterPosDir const & umPosDirTarget
    )
      : m_nobAnimated(nobAnimated),
        m_umPosDirStart(nobAnimated.GetPosDir()),
        m_umPosDirTarget(umPosDirTarget)
    {}

    virtual void Do()
    {
        //wcout << L'#' << __FUNCDNAME__ << endl;
        MicroMeterPosDir const umPosDirActual(m_nobAnimated);
        m_animation.SetNrOfSteps(CalcNrOfSteps(umPosDirActual, m_umPosDirTarget));
        m_animation.Start(umPosDirActual, m_umPosDirTarget);
    }

    virtual void Undo()
    {
        //wcout << L'#' << __FUNCDNAME__ << endl;
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
    Animation<MicroMeterPosDir> m_animation { m_applicationFunc };
};