// WrapVoltage.cpp
//
// ModelIO

module;

#include <cassert>
#include <iostream>

module WrapVoltage;

import Types;
import SaveCast;
import IoUtil;
import IoConstants;
import ErrHndl;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

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
    case synapse:
    case neuron:
    case fork:
    case knot:
        setPosNobVoltage(script, idFromScript);
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

void WrapVoltage::setPosNobVoltage
(
    Script    & script,
    NobId const idFromScript
) const
{
    PosNob & posNob { * m_modelIO.GetImportNMWI().GetNobPtr<PosNob*>(idFromScript) };
    posNob.SetVoltage(ScrReadVoltage(script));
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
    for (Pipe::SegNr segNr(0);;)
    {
        pipe.PushVoltage(ScrReadVoltage(script));
        if ((++segNr).GetValue() == nrOfSegments)
            break;
        script.ScrReadSpecial(ID_SEPARATOR);
    }
    script.ScrReadSpecial(CLOSE_BRACKET);
}

void WrapVoltage::writePipeVoltage(wostream & out, Pipe const & pipe) const
{
    Pipe::SegNr const lastSeg(Cast2Int(pipe.GetNrOfSegments()) - 1);
    out << OPEN_BRACKET 
        << pipe.GetNrOfSegments()
        << NR_SEPARATOR;
    for (auto i = Pipe::SegNr(0);; ++i)
    {
        out << pipe.GetVoltage(i).GetValue();
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
        out << m_modelIO.GetCompactIdVal(nob.GetId()) << SPACE << nob.GetTypeName() << SPACE;
        switch (nob.GetNobType().GetValue())
        {
        case NobType::Value::inputLine:
        case outputLine:
        case neuron:
        case knot:
        case fork:
        case synapse:
            out << static_cast<PosNob const &>(nob).GetVoltage().GetValue();
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
