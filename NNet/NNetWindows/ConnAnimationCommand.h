// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Command.h"
#include "Connector.h"
#include "ShapePtrList.h"
#include "MicroMeterPointVector.h"
#include "win32_callable.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class ConnAnimationCommand;
class WinCommands;
class RootWindow;

class ConnAnimationCommand : public Command
{
public:
    ConnAnimationCommand
    (
        RootWindow  &,
        WinCommands &
    );
    virtual ~ConnAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

private:

    enum class Mode
    {
        mode_do,
        mode_undo
    };
    
    Mode                             m_mode { Mode::mode_do };
    UPShapeList                    * m_pModelShapes;
    WinCommands                    & m_winCommands;
    RootWindow                     & m_win;
    Radian                           m_radianTarget;
    int                              m_iPhase { 0 };
    Callable                         m_callable;
    unique_ptr<Connector>            m_upConnector {};  
    MicroMeterLine                   m_line { MicroMeterLine::NULL_VAL() };
    MicroMeterPointVector            m_umPntVectorOriginal;
    MicroMeterPointVector            m_umPntVectorTarget1;
    MicroMeterPointVector            m_umPntVectorTarget2;
    ShapePtrList<ConnNeuron>         m_shapesAnimated {};
    Animation<MicroMeterPointVector> m_connAnimation 
    {
        Animation<MicroMeterPointVector>
        (
            [&](bool const bTargetReached) 
            { 
                m_callable.Call_UI_thread([&](){updateUI();});
                if (bTargetReached)
                    m_callable.Call_UI_thread([&](){ nextAnimationPhase(); });
            }
        )
    };

    ShapeType    const determineShapeType() const;
    unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
    void               nextAnimationPhase();
    void               updateUI();
    void               blockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true); };
    void               unblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };
};