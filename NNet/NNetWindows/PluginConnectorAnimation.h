// PluginConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "vector"
#include "MoreTypes.h"
#include "PluginAnimation.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "NobPtrList.h"
#include "SingleNobAnimation.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "ConnectIoObjectsCommand.h"

using std::unique_ptr;
using std::vector;

using ConnectConnectorsCommand = ConnectIoObjectsCommand<Connector,ClosedConnector>;

class PluginConnectorAnimation : public PluginAnimation
{
public:
    PluginConnectorAnimation
    (
        Connector   & nobAnimated,
        Connector   & nobTarget,
        MainWindow  & win,
        WinCommands & cmds
    )
        : PluginAnimation( nobAnimated, nobTarget, win, cmds )
    {
        SetConnectionCommand(move(make_unique<ConnectConnectorsCommand>(nobAnimated, nobTarget)));

        SetTarget(5.0f);
        SetTarget(1.4f);
    }

    virtual ~PluginConnectorAnimation() {};

private:
};