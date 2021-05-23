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
  : PluginAnimation(nobAnimated, win, cmds)
{
    m_upConnectionCommand = make_unique<ConnectIoNeuronsCommand>(nobAnimated, nobTarget);

    pushTarget(calcOffsetTarget(nobTarget, 3.0f));
    pushTarget(calcOffsetTarget(nobTarget, 1.4f));
}
