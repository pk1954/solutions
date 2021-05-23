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
    : PluginAnimation(nobAnimated, win, cmds)
{
    m_upConnectionCommand = make_unique<ConnectConnectorsCommand>(nobAnimated, nobTarget);

    pushTarget(calcOffsetTarget(nobTarget, 5.0f));
    pushTarget(calcOffsetTarget(nobTarget, 1.4f));
}
