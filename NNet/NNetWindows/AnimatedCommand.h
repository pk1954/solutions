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
        doPhase();
    }

    virtual void Undo( NNetModelWriterInterface& nmwi )
    {
        m_mode = Mode::mode_undo;
        m_iPhase = Cast2Int(m_umPosDirTarget.size());
        undoPhase();
    }

protected:
    enum class Mode
    {
        mode_do,
        mode_undo
    };

    vector<MicroMeterPosDir> m_umPosDirTarget;  
    int                      m_iPhase { 0 };

    Animation<MicroMeterPosDir> m_animation 
    {
        Animation<MicroMeterPosDir>
        (
            [&](bool const bTargetReached) 
            { 
                m_callable.Call_UI_thread([&](){ updateUI(); });
                if (bTargetReached)
                    m_callable.Call_UI_thread
                    (
                        [&]()
                        { 
                            if (m_mode == Mode::mode_do) 
                                doPhase();
                            else
                                undoPhase();
                        }
                );
            }
        )
    };

    NNetModelWriterInterface & m_NMWI;
    MainWindow               & m_win;
    Callable                   m_callable;

    void BlockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true);  };
    void UnblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };

private:
    Mode m_mode { Mode::mode_do };

    virtual void doPhase  () = 0;
    virtual void undoPhase() = 0;
    virtual void updateUI () = 0;
};
