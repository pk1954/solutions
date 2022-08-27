// WrapVoltage.cpp
//
// ModelIO

module;

#include <iostream>
#include "Nob.h"
#include "NobType.h"
#include "BaseKnot.h"
#include "Pipe.h"
#include "IoConnector.h"

module WrapVoltage;

import BasicTypes;
import IoConstants;
import ErrHndl;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;

using std::wostream;
using std::wcout;
using std::endl;

void WrapVoltage::setVoltage(Script & script) const
{
    NobId   const idFromScript { ScrReadNobId(script) };
    NobType const nobType      { ScrReadNobType(script) };
    switch (nobType.GetValue())
    {
    using enum NobType::Value;
    case inputLine:
    case outputLine:
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
    BaseKnot & baseKnot { * m_modelIO.GetImportNMWI().GetNobPtr<BaseKnot*>(idFromScript) };
    baseKnot.SetVoltage(ScrReadVoltage(script));
}

void WrapVoltage::setPipeVoltage
(
    Script    & script,
    NobId const idFromScript
) const
{
    Pipe & pipe { * m_modelIO.GetImportNMWI().GetNobPtr<Pipe*>(idFromScript) };
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

void WrapVoltage::writePipeVoltage(wostream & out, Pipe const & pipe) const
{
    Pipe::SegNr const lastSeg(pipe.GetNrOfSegments() - 1);
    out << OPEN_BRACKET 
        << pipe.GetNrOfSegments()
        << NR_SEPARATOR;
    for (auto i = Pipe::SegNr(0);; ++i)
    {
        out << pipe.GetVoltage(i); 
        if (i == lastSeg)
            break;
        out << ID_SEPARATOR;
    }
    out << CLOSE_BRACKET;
}

void WrapVoltage::writeVoltage(wostream & out, Nob const & nob) const 
{
    using enum NobType::Value;
    if (nob.IsDefined())
    {
        WriteCmdName(out);
        out << m_modelIO.GetCompactIdVal(nob.GetId()) << L" " << nob.GetName();
        switch (nob.GetNobType().GetValue())
        {
        case NobType::Value::inputLine:
        case outputLine:
        case neuron:
        case knot:
            out << static_cast<BaseKnot const &>(nob).GetVoltage();
            break;

        case pipe:
            writePipeVoltage(out, static_cast<Pipe const &>(nob));
            break;

        case inputConnector:
        case outputConnector:
            break;

        default:
            assert(false);
            break;
        }
        out << endl;
    }
};
