// AlignDirectionCommand.h
//
// NNetWindows

#pragma once

#include "CalcOrthoVector.h"
#include "ConnAnimationCommand.h"

class RootWindow;

class AlignDirectionCommand : public ConnAnimationCommand
{
public:
    AlignDirectionCommand(RootWindow * const pWin, NNetModelCommands * const pCmds)
        : ConnAnimationCommand(pWin, pCmds)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, m_shapesAnimated)));
    }
};