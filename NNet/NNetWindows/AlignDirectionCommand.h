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
    AlignDirectionCommand
    (
        unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated,
        RootWindow                   const * pWin,
        int                          const   iMsg,
        bool                         const   bBackwards
    )
    : ConnAnimationCommand(move(upShapesAnimated), pWin, iMsg, bBackwards)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, *m_upShapesAnimated.get())));
    }
};