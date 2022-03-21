// WrapVoltage.cpp
//
// ModelIO

#include "stdafx.h"
#include "SCRIPT.H"
#include "ERRHNDL.H"
#include "Nob.h"
#include "NobType.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "IoConnector.h"
#include "NNetWrapperHelpers.h"
#include "WrapVoltage.h"

using std::wcout;

void WrapVoltage::setVoltage(Script & script) const
{
    NobId   const idFromScript { ScrReadNobId(script) };
    NobType const nobType      { ScrReadNobType(script) };
    switch (nobType.GetValue())
    {
    using enum NobType::Value;
    case inputNeuron:
    case outputNeuron:
    case neuron:
    case knot:
        setBaseKnotVoltage(script, idFromScript);
        break;

    case pipe: 
        setPipeVoltage(script, idFromScript);
        break;

    case inputConnector:
    case outputConnector:
        break;

    default:
        assert(false);
        break;
    }
}

void WrapVoltage::setBaseKnotVoltage
(
    Script    & script,
    NobId const idFromScript
) const
{
    BaseKnot & baseKnot { GetNobRef<BaseKnot>(idFromScript) };
    baseKnot.SetVoltage(ScrReadVoltage(script));
}

void WrapVoltage::setPipeVoltage
(
    Script    & script,
    NobId const idFromScript
) const
{
    Pipe & pipe { GetNobRef<Pipe>(idFromScript) };
    script.ScrReadSpecial(OPEN_BRACKET);
    size_t const nrOfSegments { script.ScrReadUlong() };
    pipe.SetNrOfSegments(nrOfSegments);
    script.ScrReadSpecial(NR_SEPARATOR);
    Pipe::SegNr segNr(0);
    for (;;)
    {
        mV const v {ScrReadVoltage(script)};
        pipe.SetVoltage(segNr, v);
        if ((++segNr).GetValue() == nrOfSegments)
            break;
        script.ScrReadSpecial(ID_SEPARATOR);
    }
    script.ScrReadSpecial(CLOSE_BRACKET);
}
