// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Command.h"
#include "ShapePtrList.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class RootWindow;

class ConnAnimationCommand : public Command
{
public:
    ConnAnimationCommand
    (
        unique_ptr<ShapePtrList<ConnNeuron>> const,
        RootWindow                         * const, 
        function<void(bool const)>           const
    );
    virtual ~ConnAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

    virtual void DefineTarget() = 0;

    bool const TargetReached() { return m_upConnAnimation->TargetReached(); }

protected:
    MicroMeterPointVector                m_umPntVectorTarget;
    MicroMeterLine                       m_line { MicroMeterLine::NULL_VAL() };
    unique_ptr<ShapePtrList<ConnNeuron>> m_upShapesAnimated;

private:
    RootWindow                                 * m_pWindow      { nullptr };
    bool                                         m_bInitialized { false };
    MicroMeterPointVector                        m_umPntVectorStart;
    unique_ptr<Animation<MicroMeterPointVector>> m_upConnAnimation;
    function<void(bool const)> const             m_finFunc;

    void               initialize(NNetModelWriterInterface&);

    bool               const prepareData(NNetModelWriterInterface&);
    unsigned int       const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
};