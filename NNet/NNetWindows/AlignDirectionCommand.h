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
        RootWindow                 * const   pWin, 
        function<void(bool const)>   const & finFunc
    )
    : ConnAnimationCommand(move(upShapesAnimated), pWin, finFunc)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.SetDir(Vector2Radian(CalcOrthoVector(m_line, *m_upShapesAnimated.get())));
    }
};