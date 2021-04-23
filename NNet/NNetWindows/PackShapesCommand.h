// PackShapesCommand.h
//
// NNetWindows

#pragma once

#include "ConnAnimationCommand.h"

class RootWindow;

class PackShapesCommand : public ConnAnimationCommand
{
public:
    PackShapesCommand(RootWindow * const pWin, NNetModelCommands * const pCmds)
        : ConnAnimationCommand(pWin, pCmds)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
    }
};