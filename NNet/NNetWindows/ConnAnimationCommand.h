// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Command.h"
#include "Connector.h"
#include "NobPtrList.h"
#include "MicroMeterPointVector.h"
#include "win32_callable.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;
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

    bool const IsCmdOk() { return m_bAllOk; }

private:

    enum class Mode
    {
        mode_do,
        mode_undo
    };
    
    bool                             m_bAllOk { false };
    Mode                             m_mode  { Mode::mode_do };
    UPNobList                      * m_pModelNobs;
    NNetModelWriterInterface       & m_NMWI;
    RootWindow                     & m_win;
    int                              m_iPhase { 0 };
    Callable                         m_callable;
    unique_ptr<Connector>            m_upConnector {};  
    MicroMeterPointVector            m_umPntVectorOriginal;  // before animation
    MicroMeterPointVector            m_umPntVectorTarget1;   // after position alignment
    MicroMeterPointVector            m_umPntVectorTarget2;   // after direction alignment
    MicroMeterPointVector            m_umPntVectorTarget3;   // after packing
    NobPtrList<IoNeuron>           m_nobsAnimated {};
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

    NobType      const determineNobType() const;
    unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
    void               nextAnimationPhase();
    void               updateUI();
    void               blockUI()   { m_win.SendCommand2Application(IDM_BLOCK_UI, true); };
    void               unblockUI() { m_win.SendCommand2Application(IDM_BLOCK_UI, false); };
};