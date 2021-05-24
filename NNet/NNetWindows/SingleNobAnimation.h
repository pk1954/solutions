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
        m_umPosDirTarget(umPosDirTarget),
        m_umPosDirStart(nob.GetPosDir())
    {
        m_animation.SetNrOfSteps( CalcNrOfSteps(m_umPosDirStart, m_umPosDirTarget) );
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        AnimationCmd::Do(targetReachedFunc);
        m_animation.Start(m_umPosDirStart, m_umPosDirTarget);
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        AnimationCmd::Undo(targetReachedFunc);
        m_animation.Start(m_umPosDirTarget, m_umPosDirStart);
    }

private:
    unsigned int           m_uiNrOfSteps;
    Nob                  & m_nobAnimated;
    MicroMeterPosDir const m_umPosDirStart;
    MicroMeterPosDir const m_umPosDirTarget;

    Animation<MicroMeterPosDir> m_animation 
    {
        Animation<MicroMeterPosDir>
        (
            [&](bool const bTargetReached)
            { 
                Callable callable { m_win.GetWindowHandle() };
                callable.Call_UI_thread
                (
                    [&]()
                    { 
                        m_nobAnimated.SetPosDir(m_animation.GetActual());
                        m_win.Notify(false);
                    }
                );
                if (bTargetReached && m_targetReachedFunc)
                    callable.Call_UI_thread([&](){ (m_targetReachedFunc)(); });
            }
        )
    };
};