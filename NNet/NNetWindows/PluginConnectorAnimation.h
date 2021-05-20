// PluginConnectorAnimation.h
//
// NNetWindows

#pragma once

#include "vector"
#include "MoreTypes.h"
#include "AnimatedCommand.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "NobPtrList.h"
#include "win32_callable.h"
#include "win32_animation.h"

using std::vector;

class PluginConnectorAnimation : public AnimatedCommand
{
public:
    PluginConnectorAnimation
    (
        Connector   &,
        Connector   &,
        MainWindow  &,
        WinCommands &
    );
    virtual ~PluginConnectorAnimation() {};

private:

    Connector                 & m_nobTarget;
    Connector                 & m_nobAnimated;
    unique_ptr<Connector>       m_upNobAnimated;
    unique_ptr<Connector>       m_upNobTarget;
    unique_ptr<ClosedConnector> m_upClosedNob {};

    virtual void nextAnimationPhase();
    virtual void updateUI();
};