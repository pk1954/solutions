// NobIo.cpp
//
// ModelIO

module;

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

module NobIo;

import IoUtil;
import IoConstants;
import NNetWrapperHelpers;
import RunTime;
import NNetModel;

using std::vector;
using std::wostream;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::endl;
using std::wcout;
using std::wstring;
using enum NobType::Value;

Pipe* NobIo::getPipePtr(NobId const idNob) const
{
    Pipe * pPipe { NNetModelIO::GetImportNMWI().GetNobPtr<Pipe*>(idNob) };
    if (pPipe == nullptr)
    {
        unique_ptr<Pipe> upPipe { make_unique<Pipe>() };
        pPipe = upPipe.get();
        pPipe->SetId(idNob);
        NNetModelIO::GetImportNMWI().GetUPNobs().SetNob2Slot(idNob, move(upPipe));
    }
    return pPipe;
}

///////////// Pipe /////////////////////////////////////////

void NobIo::writePipe(wostream& out, Pipe const& pipe) const
{
    out << OPEN_BRACKET
        << NNetModelIO::GetCompactIdVal(pipe.GetStartKnotId())
        << PIPE_TO
        << NNetModelIO::GetCompactIdVal(pipe.GetEndKnotId())
        << CLOSE_BRACKET;
}

Pipe* NobIo::createPipe
(
    Script&     script,
    NobId const idFromScript
) const
{
    script.ScrReadSpecial(OPEN_BRACKET);
    NobId const idStart { ScrReadNobId(script) };
    script.ScrReadSpecialString(PIPE_TO);
    NobId const idEnd { ScrReadNobId(script) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    UPNobList const& list { NNetModelIO::GetImportNMWI().GetUPNobs() };
    NNetModelIO::CheckImportedNobId(script, list, idStart);
    NNetModelIO::CheckImportedNobId(script, list, idEnd);
    if (idStart == idEnd)
    {
        wcout << L"+++ Pipe has identical start and end point" << endl;
        wcout << L"+++ " << L": " << idStart << SPACE << PIPE_TO << idEnd << endl;
        wcout << L"+++ Pipe ignored" << endl;
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Error reading Pipe"));
        return nullptr;
    }
    PosNob * const pNobStart { static_cast<PosNob *>(list.GetAt(idStart)) };
    PosNob * const pNobEnd   { static_cast<PosNob *>(list.GetAt(idEnd)) };
    assert(pNobStart);
    assert(pNobEnd);
    Pipe * pPipe { getPipePtr(idFromScript) };
    assert(pPipe);
    pPipe->SetStartPnt(pNobStart);
    pPipe->SetEndPnt(pNobEnd);
    return pPipe;
}

///////////// Knot ////////////////////////////////////////

void NobIo::writeKnot(wostream& out, Knot const& knot) const
{
    out << OPEN_BRACKET;
    out << NNetModelIO::GetCompactIdVal(knot.GetIncoming()->GetId());
    out << PIPE_TO;
    out << knot.GetPos();
    out << PIPE_TO;
    out << NNetModelIO::GetCompactIdVal(knot.GetOutgoing()->GetId());
    out << CLOSE_BRACKET;
}

UPNob NobIo::createKnot(Script& script) const
{
    script.ScrReadSpecial(OPEN_BRACKET);
    Pipe* pPipeIn { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecialString(PIPE_TO);
    MicroMeterPnt const umPos(ScrReadMicroMeterPnt(script));
    script.ScrReadSpecialString(PIPE_TO);
    Pipe* pPipeOut { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    unique_ptr<Knot> upKnot { make_unique<Knot>(umPos) };
    upKnot->AddIncoming(pPipeIn);
    upKnot->AddOutgoing(pPipeOut);
    return upKnot;
}

///////////// Fork ////////////////////////////////////////

void NobIo::writeFork(wostream& out, Fork const& fork) const
{
    out << OPEN_BRACKET
        << NNetModelIO::GetCompactIdVal(fork.GetIncoming()->GetId())
        << PIPE_TO
        << fork.GetPos()
        << PIPE_TO
        << NNetModelIO::GetCompactIdVal(fork.GetFirstOutgoing()->GetId())
        << ID_SEPARATOR
        << NNetModelIO::GetCompactIdVal(fork.GetSecondOutgoing()->GetId())
        << CLOSE_BRACKET;
}

UPNob NobIo::createFork(Script& script) const
{
    script.ScrReadSpecial(OPEN_BRACKET);
    Pipe* pPipeIn { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecialString(PIPE_TO);
    MicroMeterPnt const umPos(ScrReadMicroMeterPnt(script));
    script.ScrReadSpecialString(PIPE_TO);
    Pipe* pPipeOut1 { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(ID_SEPARATOR);
    Pipe* pPipeOut2 { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    unique_ptr<Fork> upFork { make_unique<Fork>(umPos) };
    upFork->AddIncoming(pPipeIn);
    upFork->AddOutgoing(pPipeOut1);
    upFork->AddOutgoing(pPipeOut2);
    return upFork;
}

///////////// Synapse //////////////////////////////////////

void NobIo::writeSynapse(wostream& out, Synapse const& synapse) const
{
    out << OPEN_BRACKET
        << NNetModelIO::GetCompactIdVal(synapse.GetAddPipe()->GetId())
        << ID_SEPARATOR
        << NNetModelIO::GetCompactIdVal(synapse.GetInPipe()->GetId())
        << PIPE_TO
        << synapse.GetPos()
        << PIPE_TO
        << NNetModelIO::GetCompactIdVal(synapse.GetOutPipe()->GetId())
        << CLOSE_BRACKET;
}

UPNob NobIo::createSynapse(Script& script) const
{
    script.ScrReadSpecial(OPEN_BRACKET);
    Pipe* pPipeAdd { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(ID_SEPARATOR);
    Pipe* pPipeIn { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecialString(PIPE_TO);
    unique_ptr<Synapse> upSynapse { make_unique<Synapse>(ScrReadMicroMeterPnt(script)) };
    script.ScrReadSpecialString(PIPE_TO);
    Pipe* pPipeOut { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    upSynapse->SetAddPipe(pPipeAdd);
    upSynapse->AddIncoming(pPipeIn);
    upSynapse->AddOutgoing(pPipeOut);
    return upSynapse;
}

///////////// IoConnector //////////////////////////////////

void NobIo::writeIoConnector(wostream& out, IoConnector const& conn) const
{
    assert(conn.Size() > 0);
    size_t const iLast { conn.Size() - 1 };
    out << CURLY_OPEN_BRACKET << conn.Size() << NR_SEPARATOR;
    for (size_t i = 0;; ++i)
    {
        out << NNetModelIO::GetCompactIdVal(conn.GetElem(i).GetId());
        if (i == iLast)
            break;
        out << ID_SEPARATOR;
    }
    out << CURLY_CLOSE_BRACKET;
}

UPNob NobIo::createIoConnector(Script& script, NobType const nobType) const
{
    bool bOK = true;
    vector<IoLine*> ioLineList;
    script.ScrReadSpecial(CURLY_OPEN_BRACKET);
    int const iNrOfElements { script.ScrReadInt() };
    script.ScrReadSpecial(NR_SEPARATOR);
    for (int iElem { 0 };;)
    {
        NobId const id      { ScrReadNobId(script) };
        IoLine*     pIoLine { NNetModelIO::GetImportNMWI().GetNobPtr<IoLine*>(id) };
        ioLineList.push_back(pIoLine);
        if (++iElem == iNrOfElements)
            break;
        script.ScrReadSpecial(ID_SEPARATOR);
    }
    script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
    unique_ptr<IoConnector> upIoConnector;
    if (bOK)
    {
        if (nobType.IsInputConnectorType())
            upIoConnector = make_unique<InputConnector>(move(ioLineList));
        else
            upIoConnector = make_unique<OutputConnector>(move(ioLineList));
    }
    return upIoConnector;
}

///////////// InputLine ////////////////////////////////////////

void NobIo::writeInputLine(wostream& out, InputLine const& inputLine) const
{
    out << OPEN_BRACKET
        << inputLine.GetPos()
        << PIPE_TO
        << NNetModelIO::GetCompactIdVal(inputLine.GetPipeC()->GetId())
        << CLOSE_BRACKET;
}

UPNob NobIo::createInputLine(Script& script) const
{
    unique_ptr<InputLine> upInputLine { };
    script.ScrReadSpecial(OPEN_BRACKET);
    MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
    script.ScrReadSpecialString(PIPE_TO);
    Pipe*         const pPipe { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    upInputLine = make_unique<InputLine>(umPos);
    upInputLine->SetPipe(pPipe);
    return upInputLine;
}

///////////// OutputLine ////////////////////////////////////////

void NobIo::writeOutputLine(wostream& out, OutputLine const& outputLine) const
{
    out << OPEN_BRACKET
        << NNetModelIO::GetCompactIdVal(outputLine.GetPipeC()->GetId())
        << PIPE_TO
        << outputLine.GetPos()
        << CLOSE_BRACKET;
}

UPNob NobIo::createOutputLine(Script& script) const
{
    unique_ptr<OutputLine> upOutputLine { };
    script.ScrReadSpecial(OPEN_BRACKET);
    Pipe* const pPipe { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecialString(PIPE_TO);
    MicroMeterPnt const umPos { ScrReadMicroMeterPnt(script) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    upOutputLine = make_unique<OutputLine>(umPos);
    upOutputLine->SetPipe(pPipe);
    return upOutputLine;
}

///////////// Neuron ////////////////////////////////////////

void NobIo::writeNeuron(wostream& out, Neuron const& neuron) const
{
    size_t const iLast { neuron.GetNrOfInConns() - 1 };
    out << OPEN_BRACKET;
    out << CURLY_OPEN_BRACKET << neuron.GetNrOfInConns() << NR_SEPARATOR;
    for (size_t i = 0;; ++i)
    {
        out << NNetModelIO::GetCompactIdVal(neuron.GetIncoming(i)->GetId());
        if (i == iLast)
            break;
        out << ID_SEPARATOR;
    }
    out << CURLY_CLOSE_BRACKET
        << PIPE_TO
        << neuron.GetPos()
        << PIPE_TO
        << NNetModelIO::GetCompactIdVal(neuron.GetAxon()->GetId())
        << CLOSE_BRACKET;
}

UPNob NobIo::createNeuron(Script& script) const
{
    PipeList pipeListIn;
    script.ScrReadSpecial(OPEN_BRACKET);
    script.ScrReadSpecial(CURLY_OPEN_BRACKET);
    int const iNrOfElements { script.ScrReadInt() };
    script.ScrReadSpecial(NR_SEPARATOR);
    for (int iElem { 0 };;)
    {
        Pipe * const pPipeIn { getPipePtr(ScrReadNobId(script)) };
        pipeListIn.Add(*pPipeIn);
        if (++iElem == iNrOfElements)
            break;
        script.ScrReadSpecial(ID_SEPARATOR);
    }
    script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
    script.ScrReadSpecialString(PIPE_TO);
    MicroMeterPnt const umPos(ScrReadMicroMeterPnt(script));
    script.ScrReadSpecialString(PIPE_TO);
    Pipe * pPipeAxon { getPipePtr(ScrReadNobId(script)) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    unique_ptr<Neuron> upNeuron { make_unique<Neuron>(umPos) };
    pipeListIn.Apply2All([&upNeuron](Pipe& pipe) { upNeuron->AddIncoming(&pipe); });
    upNeuron->SetAxon(pPipeAxon);
    return upNeuron;
}

///////////// Nob ///////////////////////////////////////////

void NobIo::writeNob(wostream& out, Nob const& nob) const
{
    if (nob.IsDefined())
    {
        WriteCmdName(out);
        out << NNetModelIO::GetCompactIdVal(nob.GetId()) << SPACE << nob.GetTypeName();
        switch (nob.GetNobType().GetValue())
        {
        case inputLine:
            writeInputLine(out, static_cast<InputLine const&>(nob));
            break;

        case outputLine:
            writeOutputLine(out, static_cast<OutputLine const&>(nob));
            break;

        case neuron:
            writeNeuron(out, static_cast<Neuron const&>(nob));
            break;

        case knot:
            writeKnot(out, static_cast<Knot const&>(nob));
            break;

        case fork:
            writeFork(out, static_cast<Fork const&>(nob));
            break;

        case pipe:
            writePipe(out, static_cast<Pipe const&>(nob));
            break;

        case synapse:
            writeSynapse(out, static_cast<Synapse const&>(nob));
            break;

        case inputConnector:
        case outputConnector:
            writeIoConnector(out, static_cast<IoConnector const&>(nob));
            break;

        default:
            assert(false);
            break;
        }
        out << endl;
    }
}

Nob* NobIo::createNob(Script& script) const
{
    NobId   const idFromScript { ScrReadNobId  (script) };
    NobType const nobType      { ScrReadNobType(script) };
    UPNob         upNob;

    switch (nobType.GetValue())
    {
    case inputConnector:
    case outputConnector:
        upNob = createIoConnector(script, nobType);
        break;
    case inputLine:
        upNob = createInputLine(script);
        break;
    case outputLine:
        upNob = createOutputLine(script);
        break;
    case neuron:
        upNob = createNeuron(script);
        break;
    case fork:
        upNob = createFork(script);
        break;
    case knot:
        upNob = createKnot(script);
        break;
    case synapse:
        upNob = createSynapse(script);
        break;
    case pipe:
        return createPipe(script, idFromScript);
    default:
        assert(false);  // handled in ScrReadNobType
        break;
    }

    Nob* pNob { nullptr };
    if (upNob)
    {
        UPNobList& list { NNetModelIO::GetImportNMWI().GetUPNobs() };
        upNob->SetId(idFromScript);
        pNob = upNob.get();
        list.SetNob2Slot(idFromScript, move(upNob));
    }

    if (pNob && nobType.IsIoConnectorType())
    {
        static_cast<IoConnector*>(pNob)->ConnectIoLines();
    }

    return pNob;
}
