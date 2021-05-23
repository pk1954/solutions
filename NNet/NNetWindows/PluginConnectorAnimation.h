// PluginConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "PluginAnimation.h"
#include "ConnectIoObjectsCommand.h"

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
        SetConnectionCommand(move(make_unique<ConnectIoObjectsCommand<Connector,ClosedConnector>>(nobAnimated, nobTarget)));

        SetTarget(5.0f);
        SetTarget(1.4f);
    }

    virtual ~PluginConnectorAnimation() {};

private:
};