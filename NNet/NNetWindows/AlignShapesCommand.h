// AlignShapesCommand.h
//
// NNetWindows

#pragma once

#include "ConnAnimationCommand.h"

class RootWindow;

class AlignShapesCommand : public ConnAnimationCommand
{
public:
    AlignShapesCommand
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
        m_umPntVectorTarget.Align(m_line);
    }
};