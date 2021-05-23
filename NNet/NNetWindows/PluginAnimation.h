// PluginAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPntVector.h"
#include "Command.h"
#include "win32_commands.h"
#include "SingleNobAnimation.h"

class NNetModelWriterInterface;
class WinCommands;
class MainWindow;
class Nob;

class PluginAnimation : public Command
{
public:
    PluginAnimation(Nob &, Nob &, MainWindow &, WinCommands & );
    
    virtual void Do  ( NNetModelWriterInterface & );
    virtual void Undo( NNetModelWriterInterface & );

protected:
    void SetTarget(float);
    void SetConnectionCommand( unique_ptr<Command> );

private:
    unique_ptr<SingleNobAnimation> m_upSingleNobAnimation;
    unique_ptr<Command>            m_upConnectionCommand;

    NNetModelWriterInterface & m_NMWI;
    MainWindow               & m_win;
    Nob                      & m_nobAnimated;
    Nob                      & m_nobTarget;
    MicroMeterPntVector        m_umPosDirTarget { };  
    unsigned int               m_uiPhase        { 0 };

    void BlockUI();
    void UnblockUI();

    void doPhase();
    void undoPhase();

    MicroMeterPosDir const getTarget(unsigned int const);
    void pushTarget(MicroMeterPosDir const &);
};
