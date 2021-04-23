// AlignShapesCommand.h
//
// NNetWindows

#pragma once

#include "ConnAnimationCommand.h"

class RootWindow;

class AlignShapesCommand : public ConnAnimationCommand
{
public:
    AlignShapesCommand(RootWindow * const pWin, NNetModelCommands * const pCmds)
        : ConnAnimationCommand(pWin, pCmds)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Align(m_line);
    }
};