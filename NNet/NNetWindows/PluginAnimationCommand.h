// PluginAnimationCommand.h
//
// NNetWindows

#pragma once

#include "array"
#include "MoreTypes.h"
#include "Command.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "ShapePtrList.h"
#include "win32_mainWindow.h"
#include "win32_callable.h"
#include "win32_animation.h"

using std::array;

class NNetModelWriterInterface;
class NNetModelCommands;
class WinCommands;

class PluginAnimationCommand : public Command
{
public:
    PluginAnimationCommand
    (
        Connector   &,
        Connector   &,
        MainWindow  &,
        WinCommands &
    );
    virtual ~PluginAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

private:

    enum class Mode
    {
        mode_do,
        mode_undo
    };

    Mode                        m_mode { Mode::mode_do };
    UPShapeList               * m_pModelShapes;
    NNetModelWriterInterface  & m_NMWI;
    MainWindow                & m_win;
    int                         m_iPhase { 0 };
    Callable                    m_callable;
    Connector                 & m_connTarget;
    Connector                 & m_connAnimated;
    array<MicroMeterPosDir,3>   m_umPosDirTarget;  
    Connector                   m_connectorAnimated {};
    unique_ptr<ClosedConnector> m_upClosedConnector {};
    Animation<MicroMeterPosDir> m_pluginAnimation 
    {
        Animation<MicroMeterPosDir>
        (
            [&](bool const bTargetReached) 
            { 
                m_callable.Call_UI_thread([&](){updateUI();});
                if (bTargetReached)
                    m_callable.Call_UI_thread([&](){ nextAnimationPhase(); });
            }
        )
    };

    unsigned int const calcNrOfSteps(MicroMeterPosDir const &, MicroMeterPosDir const &) const;
    void               nextAnimationPhase();
    void               updateUI();
    void               blockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true); };
    void               unblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };
};