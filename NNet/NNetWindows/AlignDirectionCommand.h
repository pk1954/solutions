// AlignDirectionCommand.h
//
// NNetWindows

#pragma once

#include "win32_Commands.h"
#include "CalcOrthoVector.h"
#include "ConnAnimationCommand.h"

class RootWindow;

class AlignDirectionCommand : public ConnAnimationCommand
{
public:
    AlignDirectionCommand
    (
        RootWindow  * pWin,
        WinCommands & cmds,
        int   const   iMsg,
        bool  const   bBackwards
    )
    : ConnAnimationCommand(move(cmds.CreateShapeList()), pWin, cmds, iMsg, bBackwards)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, *m_upShapesAnimated.get())));
    }
};