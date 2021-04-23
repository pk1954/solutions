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
    ConnAnimationCommand(RootWindow * const, NNetModelCommands * const);
    virtual ~ConnAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

    virtual void DefineTarget() = 0;

    bool const TargetReached() { return m_upConnAnimation->TargetReached(); }

protected:
    using ConnAnimation = Animation<MicroMeterPointVector>;

    MicroMeterPointVector     m_umPntVectorTarget;
    MicroMeterLine            m_line { MicroMeterLine::NULL_VAL() };
    ShapePtrList<ConnNeuron>  m_shapesAnimated;

private:
    RootWindow              * m_pWindow        { nullptr };
    bool                      m_bInitialized   { false };
    NNetModelCommands       * m_pModelCommands { nullptr };
    MicroMeterPointVector     m_umPntVectorStart;
    unique_ptr<ConnAnimation> m_upConnAnimation;

    void               initialize(NNetModelWriterInterface&);
    bool         const prepareData(NNetModelWriterInterface&);
    unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
};