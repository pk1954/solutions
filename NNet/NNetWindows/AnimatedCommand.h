// AnimatedCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPntVector.h"
#include "Command.h"
#include "win32_mainWindow.h"
#include "win32_animation.h"
#include "win32_callable.h"
#include "win32_commands.h"

class NNetModelWriterInterface;

class AnimatedCommand : public Command
{
public:
    AnimatedCommand
    ( 
        MainWindow  & win,
        WinCommands & cmds
    )
    :   m_win(win),
        m_NMWI(cmds.GetNMWI()),
        m_callable(win.GetWindowHandle())
    {}

    virtual void Do( NNetModelWriterInterface& nmwi )
    {
        m_mode = Mode::mode_do;
        m_iPhase = 1;
        nextAnimationPhase();
    }

    void Undo( NNetModelWriterInterface& nmwi )
    {
        m_mode = Mode::mode_undo;
        m_iPhase = Cast2Int(m_umPosDirTarget.size());
        nextAnimationPhase();
    }

protected:
    enum class Mode
    {
        mode_do,
        mode_undo
    };

    vector<MicroMeterPosDir> m_umPosDirTarget;  
    Mode                     m_mode   { Mode::mode_do };
    int                      m_iPhase { 0 };

    Animation<MicroMeterPosDir> m_animation 
    {
        Animation<MicroMeterPosDir>
        (
            [&](bool const bTargetReached) 
            { 
                m_callable.Call_UI_thread([&](){ updateUI(); });
                if (bTargetReached)
                    m_callable.Call_UI_thread([&](){ nextAnimationPhase(); });
            }
        )
    };

    NNetModelWriterInterface & m_NMWI;
    MainWindow               & m_win;
    Callable                   m_callable;

    void BlockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true);  };
    void UnblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };

    void StartAnimation
    (
        MicroMeterPosDir const & umPosDirStart,
        MicroMeterPosDir const & umPosDirTarget
    )
    {
        m_animation.SetNrOfSteps( calcNrOfSteps(umPosDirStart, umPosDirTarget) );
        m_animation.Start(umPosDirStart, umPosDirTarget);
        m_win.Notify(false);
    }

private:
    virtual void nextAnimationPhase() = 0;
    virtual void updateUI()           = 0;

    unsigned int const calcNrOfSteps
    (
        MicroMeterPosDir const & umPosDirStart,
        MicroMeterPosDir const & umPosDirTarget
    ) const
    {
        MicroMeterPosDir const umPosDirDiff   { umPosDirTarget - umPosDirStart };

        Radian           const radPerStep     { Degrees2Radian(6.0_Degrees) };
        float            const fStepsFromRot  { Normalize(umPosDirDiff.GetDir()) / radPerStep };

        MicroMeter       const umPerStep      { NEURON_RADIUS / 5.0f };
        float            const fStepsFromMove { Hypot(umPosDirDiff.GetPos()) / umPerStep };

        float            const fSteps         { max(fStepsFromRot, fStepsFromMove) };
        unsigned int     const uiSteps        { Cast2UnsignedInt(fSteps) + 1 };
        return uiSteps;
    }
};
