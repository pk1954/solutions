// PluginAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetModelWriterInterface.h"
#include "win32_Commands.h"
#include "win32_mainWindow.h"
#include "SingleNobAnimation.h"
#include "PluginAnimation.h"

PluginAnimation::PluginAnimation
( 
    Nob         & nobAnimated,
    Nob         & nobTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : m_nobAnimated(nobAnimated),
    m_nobTarget  (nobTarget),
    m_win(win),
    m_NMWI(cmds.GetNMWI())
{
}

void PluginAnimation::Do( NNetModelWriterInterface& nmwi )
{
    m_uiPhase = 0;
    doPhase();
}

void PluginAnimation::Undo( NNetModelWriterInterface& nmwi )
{
    m_uiPhase = Cast2Int(m_moveSteps.size());
    undoPhase();
}

void PluginAnimation::SetTarget(float fOffset)
{
    if (m_nobTarget.GetIoMode() == NobIoMode::input )
        fOffset = -fOffset;
    Radian           const radianTarget { m_nobTarget.GetDir() };
    MicroMeterPnt    const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };
    MicroMeterPnt    const umPosOffset  { umDirVector * fOffset };
    MicroMeterPnt    const umPosTarget  { m_nobTarget.GetPos() + umPosOffset };
    MicroMeterPosDir const umPosDirTarget(umPosTarget, radianTarget);
    m_moveSteps.push_back(make_unique<SingleNobAnimation>(m_win, m_nobAnimated, umPosDirTarget));
}

void PluginAnimation::SetConnectionCommand( unique_ptr<Command> upCmd )
{
    m_upConnectionCommand = move(upCmd);
}

void PluginAnimation::BlockUI()   
{ 
    m_win.SendCommand2Application(IDM_BLOCK_UI, true);  
};

void PluginAnimation::UnblockUI()
{ 
    m_win.SendCommand2Application(IDM_BLOCK_UI, false); 
};

void PluginAnimation::doPhase() // runs in UI thread
{
    switch (m_uiPhase++)
    {
    case 0:  
        BlockUI();
        doPhase();
        break;

    case 1:  
        m_moveSteps[0]->Do([&](){ doPhase(); });
        break;

    case 2:	 
        m_moveSteps[1]->Do([&](){ doPhase(); });
        break;

    case 3:  
        m_upConnectionCommand->Do(m_NMWI);
        m_win.Notify(false);
        UnblockUI();
        break; 

    default: 
        break;        
    }
}

void PluginAnimation::undoPhase() // runs in UI thread
{
    switch (m_uiPhase--)
    {
    case 3:  
        BlockUI();
        m_upConnectionCommand->Undo(m_NMWI);
        m_win.Notify(false);
        undoPhase();
        break;

    case 2:  
        m_moveSteps[1]->Undo([&](){ undoPhase(); });
        break;

    case 1:	 
        m_moveSteps[0]->Undo([&](){ undoPhase(); });
        break;

    case 0:  
        UnblockUI();
        break; 

    default: 
        break;
    }
}
