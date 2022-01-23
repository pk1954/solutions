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
        {
            BaseKnot & baseKnot { GetNobRef<BaseKnot>(idFromScript) };
            baseKnot.SetVoltage(ScrReadVoltage(script));
        }
        break;

    case pipe: 
        setPipeVoltage(script, idFromScript);
        break;

    case inputConnector:
    case outputConnector:
        setIoConnectorVoltage(script, idFromScript);
    break;

    default:
        assert(false);
        break;
    }
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
    if (nrOfSegmentsFromScript == nrOfSegmentsFromModel)
    {
        Pipe::SegNr segNr(0);
        for (;;)
        {
            mV const v {ScrReadVoltage(script)};
            pipe.SetVoltage(segNr, v);
            if ((++segNr).GetValue() == nrOfSegmentsFromScript)
                break;
            script.ScrReadSpecial(BaseKnot::NR_SEPARATOR);
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

void WrapVoltage::setIoConnectorVoltage
(
    Script    & script,
    NobId const idFromScript
) const
{
    IoConnector & ioConnector { GetNobRef<IoConnector>(idFromScript) };
    script.ScrReadSpecial(BaseKnot::OPEN_BRACKET);
    size_t const nrOfElementsFromScript { script.ScrReadUlong() };
    size_t const nrOfElementsFromModel  { ioConnector.GetNrOfElements() };
    if (nrOfElementsFromScript == nrOfElementsFromModel)
    {
        size_t index(0);
        for (;;)
        {
            mV const v {ScrReadVoltage(script)};
            ioConnector.GetElem(index).SetVoltage(v);
            if (++index == nrOfElementsFromScript)
                break;
            script.ScrReadSpecial(BaseKnot::NR_SEPARATOR);
        }
    }
    else
    {
        wcout << "+++ Wrong number of IoConnector elements" << endl;
        wcout << "+++ " << nrOfElementsFromScript << L" != " << nrOfElementsFromModel << endl;
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Error reading IoConnectorr voltage") );
    }
    script.ScrReadSpecial(Pipe::CLOSE_BRACKET);
}
