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
        unique_ptr<ShapePtrList<ConnNeuron>>                 upShapesAnimated,
        function<void(ConnAnimationCommand const *)> const & func
    )
        : ConnAnimationCommand(move(upShapesAnimated), func)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Align(m_line);
    }
};