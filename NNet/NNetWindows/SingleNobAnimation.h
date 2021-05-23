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

using std::function;

class SingleNobAnimation
{
public:
    SingleNobAnimation
    ( 
        MainWindow & win,
        Nob        & nob
    )
      : m_win(win),
        m_nobAnimated(nob)
    {}

    void Start
    (
        MicroMeterPosDir const & umPosDirTarget,
        function<void()> const   targetReachedFunc
    )
    {
        MicroMeterPosDir umPosDirStart { m_nobAnimated.GetPosDir() };
        m_targetReachedFunc = targetReachedFunc;
        m_animation.SetNrOfSteps( CalcNrOfSteps(umPosDirStart, umPosDirTarget) );
        m_animation.Start(umPosDirStart, umPosDirTarget);
    }

    MicroMeterPosDir const GetActual()
    {
        return m_animation.GetActual();
    }

private:
    Nob            & m_nobAnimated;
    MainWindow     & m_win;
    function<void()> m_targetReachedFunc { nullptr };

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
                if (bTargetReached)
                    callable.Call_UI_thread([&](){ (m_targetReachedFunc)(); });
            }
        )
    };
};