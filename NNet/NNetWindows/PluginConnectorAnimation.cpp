// PluginConnectorAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "MoreTypes.h"
#include "CalcOrthoVector.h"
#include "win32_Commands.h"
#include "CommandStack.h"
#include "NobIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "PluginConnectorAnimation.h"

using std::make_unique;

PluginConnectorAnimation::PluginConnectorAnimation
( 
    Connector   & nobAnimated,
    Connector   & nobTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : AnimatedCommand(win, cmds),
    m_nobAnimated(nobAnimated)
{
    m_upSingleNobAnimation = make_unique<SingleNobAnimation>(win, nobAnimated);
    m_upConnectConnectors  = make_unique<ConnectConnectorsCommand>(nobAnimated, nobTarget);

    Radian        const radianTarget { nobTarget.GetDir() };
    MicroMeterPnt const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };
    
    m_umPosDirTarget.push_back(nobAnimated.GetPosDir());

    array <float, 2> fOffsets { 5.0f, 1.4f };

    for (size_t i = 1; i<= 2; ++i )
    {
        float fOff { fOffsets[i-1] };
        if (nobTarget.GetIoMode() == NobIoMode::input )
            fOff = -fOff;
        MicroMeterPnt const umPosOffset { umDirVector * fOff };
        MicroMeterPnt const umPosTarget { nobTarget.GetPos() + umPosOffset };
        m_umPosDirTarget.push_back(MicroMeterPosDir(umPosTarget, radianTarget));
    }
}

void PluginConnectorAnimation::updateUI()  // runs in animation thread
{
    m_nobAnimated.SetPosDir(m_animation.GetActual());
    m_win.Notify(false);
}

void PluginConnectorAnimation::doPhase() // runs in UI thread
{
    switch (m_iPhase++)
    {
    case 0:  BlockUI();
             doPhase();
             break;

    case 1:  m_upSingleNobAnimation->Start(m_umPosDirTarget[1], [&](){ doPhase(); });
             break;

    case 2:	 m_upSingleNobAnimation->Start(m_umPosDirTarget[2], [&](){ doPhase(); });
             break;

    case 3:  m_upConnectConnectors->Do(m_NMWI);
             m_win.Notify(false);
             UnblockUI();
             break; 

    default: break;
    }
}

void PluginConnectorAnimation::undoPhase() // runs in UI thread
{
    switch (m_iPhase--)
    {
    case 3:  BlockUI();
             m_upConnectConnectors->Undo(m_NMWI);
             undoPhase();
             break;

    case 2:  m_upSingleNobAnimation->Start(m_umPosDirTarget[1], [&](){ undoPhase(); });
             break;

    case 1:	 m_upSingleNobAnimation->Start(m_umPosDirTarget[0], [&](){ undoPhase(); });
             break;

    case 0:  UnblockUI();
             break; 

    default: break;
    }
}
