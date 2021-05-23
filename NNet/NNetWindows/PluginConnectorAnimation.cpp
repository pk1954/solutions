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
  : PluginAnimation(win, cmds),
    m_nobAnimated(nobAnimated)
{
    m_upSingleNobAnimation = make_unique<SingleNobAnimation>(win, nobAnimated);
    m_upConnectConnectors  = make_unique<ConnectConnectorsCommand>(nobAnimated, nobTarget);

    pushTarget(m_nobAnimated.GetPosDir());
    pushTarget(calcOffsetTarget(nobTarget, 5.0f));
    pushTarget(calcOffsetTarget(nobTarget, 1.4f));
}

void PluginConnectorAnimation::updateUI()  // runs in animation thread
{
    m_nobAnimated.SetPosDir(m_upSingleNobAnimation->GetActual());
    m_win.Notify(false);
}

void PluginConnectorAnimation::doPhase(unsigned int const uiPhase) // runs in UI thread
{
    switch (uiPhase)
    {
    case 0:  BlockUI();
             doPhase(1);
             break;

    case 1:  m_upSingleNobAnimation->Start(getTarget(1), [&](){ doPhase(2); });
             break;

    case 2:	 m_upSingleNobAnimation->Start(getTarget(2), [&](){ doPhase(3); });
             break;

    case 3:  m_upConnectConnectors->Do(m_NMWI);
             m_win.Notify(false);
             UnblockUI();
             break; 

    default: break;
    }
}

void PluginConnectorAnimation::undoPhase(unsigned int const uiPhase) // runs in UI thread
{
    switch (uiPhase)
    {
    case 3:  BlockUI();
             m_upConnectConnectors->Undo(m_NMWI);
             undoPhase(2);
             break;

    case 2:  m_upSingleNobAnimation->Start(getTarget(1), [&](){ undoPhase(1); });
             break;

    case 1:	 m_upSingleNobAnimation->Start(getTarget(0), [&](){ undoPhase(0); });
             break;

    case 0:  UnblockUI();
             break; 

    default: break;
    }
}
