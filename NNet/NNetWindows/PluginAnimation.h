// PluginAnimation.h
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

class PluginAnimation : public Command
{
public:
    PluginAnimation
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
        m_uiPhase = 0;
        doPhase(m_uiPhase);
    }

    virtual void Undo( NNetModelWriterInterface& nmwi )
    {
        m_uiPhase = Cast2Int(m_umPosDirTarget.size());
        undoPhase(m_uiPhase);
    }

protected:
    MicroMeterPosDir const calcOffsetTarget(Nob const & nobTarget, float fOffset)
    {
        if (nobTarget.GetIoMode() == NobIoMode::input )
            fOffset = -fOffset;
        Radian        const radianTarget { nobTarget.GetDir() };
        MicroMeterPnt const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };
        MicroMeterPnt const umPosOffset  { umDirVector * fOffset };
        MicroMeterPnt const umPosTarget  { nobTarget.GetPos() + umPosOffset };
        return MicroMeterPosDir(umPosTarget, radianTarget);
    }

    void pushTarget( MicroMeterPosDir const& umPosDir )
    {
        m_umPosDirTarget.push_back(umPosDir);
    }

    MicroMeterPosDir const& getTarget( unsigned int const uiStep )
    {
        return m_umPosDirTarget.at(uiStep);
    }

    NNetModelWriterInterface & m_NMWI;
    MainWindow               & m_win;
    Callable                   m_callable;

    void BlockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true);  };
    void UnblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };

private:

    vector<MicroMeterPosDir> m_umPosDirTarget { };  
    unsigned int             m_uiPhase        { 0 };

    virtual void doPhase  (unsigned int const) = 0;
    virtual void undoPhase(unsigned int const) = 0;
    virtual void updateUI ()                   = 0;
};
