// BaseKnotAnimation.h
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

using std::function;

class BaseKnotAnimation
{
public:
    BaseKnotAnimation
    ( 
        MainWindow win,
        BaseKnot & baseKnot
    )
      : m_win(win),
        m_baseKnotAnimated(baseKnot),
        m_callable(win.GetWindowHandle()),
        m_umPosDirStart(m_baseKnotAnimated.GetPosDir())
    {
    }

    void Start
    (
        MicroMeterPosDir const & umPosDirTarget,
        function<void()> const & targetReachedFunc
    )
    {
        m_umPosDirTarget = umPosDirTarget;
        m_pTargetReachedFunc = & targetReachedFunc;
        m_animation.SetNrOfSteps( CalcNrOfSteps(m_umPosDirStart, umPosDirTarget) );
    }

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        m_animation.Start(m_umPosDirStart, m_umPosDirTarget);
    }

    void Undo(NNetModelWriterInterface & nmwi)
    {
        m_animation.Start(m_umPosDirTarget, m_umPosDirStart);
    }

private:
    BaseKnot               & m_baseKnotAnimated;
    MainWindow             & m_win;
    Callable                 m_callable;
    MicroMeterPosDir         m_umPosDirTarget { MicroMeterPosDir::NULL_VAL() };
    MicroMeterPosDir         m_umPosDirStart { MicroMeterPosDir::NULL_VAL() };
    function<void()> const * m_pTargetReachedFunc;

    Animation<MicroMeterPosDir> m_animation 
    {
        Animation<MicroMeterPosDir>
        (
            [&](bool const bTargetReached) 
            { 
                m_callable.Call_UI_thread
                (
                    [&]()
                    { 
                        m_baseKnotAnimated.SetPosDir(m_animation.GetActual());
                        m_win.Notify(false);
                    }
                );
                if (bTargetReached)
                    m_callable.Call_UI_thread([&](){ (* m_pTargetReachedFunc)(); });
            }
        )
    };
};