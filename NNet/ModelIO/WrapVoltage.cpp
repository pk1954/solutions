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
    script.ScrReadSpecial(Pipe::OPEN_BRACKET);
    size_t const nrOfSegmentsFromScript { script.ScrReadUlong() };
    size_t const nrOfSegmentsFromModel  { pipe.GetNrOfSegments() };
    script.ScrReadSpecial(BaseKnot::NR_SEPARATOR);
    if (nrOfSegmentsFromScript == nrOfSegmentsFromModel)
    {
        Pipe::SegNr segNr(0);
        for (;;)
        {
            mV const v {ScrReadVoltage(script)};
            pipe.SetVoltage(segNr, v);
            if ((++segNr).GetValue() == nrOfSegmentsFromScript)
                break;
            script.ScrReadSpecial(Pipe::ID_SEPARATOR);
        }
    }
    else
    {
        wcout << "+++ Wrong number of pipe segments" << endl;
        wcout << "+++ " << nrOfSegmentsFromScript << L" != " << nrOfSegmentsFromModel << endl;
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Error reading Pipe voltage") );
    }
    script.ScrReadSpecial(Pipe::CLOSE_BRACKET);
}
