// PluginIoNeuronAnimation.cpp
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
#include "ConnectIoObjectsCommand.h"
#include "PluginIoNeuronAnimation.h"

using std::make_unique;

PluginIoNeuronAnimation::PluginIoNeuronAnimation
( 
    IoNeuron    & nobAnimated,
    IoNeuron    & nobTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : PluginAnimation(win, cmds),
    m_nobAnimated(nobAnimated)
{
    m_upSingleNobAnimation = make_unique<SingleNobAnimation>(win, nobAnimated);
    m_upConnectIoNeurons   = make_unique<ConnectIoNeuronsCommand>(nobAnimated, nobTarget);

    pushTarget(m_nobAnimated.GetPosDir());
    pushTarget(calcOffsetTarget(nobTarget, 3.0f));
    pushTarget(calcOffsetTarget(nobTarget, 1.4f));
}

void PluginIoNeuronAnimation::updateUI()  // runs in animation thread
{
    m_nobAnimated.SetPosDir(m_upSingleNobAnimation->GetActual());
    m_win.Notify(false);
}

void PluginIoNeuronAnimation::doPhase(unsigned int const uiPhase) // runs in UI thread
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

    case 3:  m_upConnectIoNeurons->Do(m_NMWI);
             m_win.Notify(false);
             UnblockUI();
             break; 

    default: break;        
    }
}

void PluginIoNeuronAnimation::undoPhase(unsigned int const uiPhase) // runs in UI thread
{
    switch (uiPhase)
    {
    case 3:  BlockUI();
             m_upConnectIoNeurons->Undo(m_NMWI);
             undoPhase(2);
             break;

    case 2:  m_upSingleNobAnimation->Start(getTarget(1), [&](){ undoPhase(1); });
             break;

    case 1:	 m_upSingleNobAnimation->Start(getTarget(0), [&](){ undoPhase(0); });
             break;

    case 0:  UnblockUI();
             break; 

    default: break;                // do not start animation
    }
}
