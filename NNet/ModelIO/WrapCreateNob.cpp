// WrapCreateNob.cpp
//
// ModelIO

#include "stdafx.h"
#include "SCRIPT.H"
#include "ERRHNDL.H"
#include "Knot.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "IoConnector.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "NNetWrapperHelpers.h"
#include "WrapCreateNob.h"

using std::wcout;

Nob * WrapCreateNob::createNob(Script & script) const
{   
    NobId   const idFromScript { ScrReadNobId(script) };
    NobType const nobType      { ScrReadNobType(script) };
    Nob         * pNob         { nullptr };
    UPNob         upNob        {};   
    { 
        switch (nobType.GetValue())
        {
        using enum NobType::Value;
        case inputConnector:
        case outputConnector:
            upNob = createIoConnector(script, nobType);
            break;
        case inputNeuron:
        case outputNeuron:
        case neuron:
        case knot:
            upNob = createBaseKnot(script, nobType);
            break;
        case pipe:
            upNob = createPipe(script);
            break;
        default:
            assert(false);  // handled in ScrReadNobType
            break;
        }
    };
    if (upNob)
    {
        upNob->SetId(idFromScript);
        pNob = upNob.get();
        GetUPNobsRef().SetNob2Slot(idFromScript, move(upNob));
    }
    if (pNob && nobType.IsIoConnectorType())
    {
        static_cast<IoConnector *>(pNob)->SetParentPointers();
    }
    return pNob;
}

UPNob WrapCreateNob::createPipe(Script & script) const
{
    script.ScrReadSpecial(OPEN_BRACKET);
    NobId const idStart { ScrReadNobId(script) };
    for (int i = 0; i < FROM_TO.size(); i++ )
        script.ScrReadSpecial(FROM_TO[i]);        
    NobId const idEnd { ScrReadNobId(script) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    NNetModelImporter::CheckImportedNobId(script, GetUPNobsRef(), idStart);
    NNetModelImporter::CheckImportedNobId(script, GetUPNobsRef(), idEnd);
    if (idStart == idEnd)
    {
        wcout << "+++ Pipe has identical start and end point" << endl;
        wcout << "+++ " << L": " << idStart << L" -> " << idEnd << endl;
        wcout << "+++ Pipe ignored" << endl;
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Error reading Pipe") );
        return nullptr;
    }
    else
    { 
        BaseKnot * const pKnotStart { GetWriterInterface().GetNobPtr<BaseKnot *>(idStart) };
        BaseKnot * const pKnotEnd   { GetWriterInterface().GetNobPtr<BaseKnot *>(idEnd  ) };
        unique_ptr<Pipe> upPipe { make_unique<Pipe>(pKnotStart, pKnotEnd, GetWriterInterface().GetParams()) };
        pKnotStart->AddOutgoing(*upPipe.get());
        pKnotEnd  ->AddIncoming(*upPipe.get());
        return move(upPipe);
    }
}

UPNob WrapCreateNob::createBaseKnot(Script & script, NobType const nobType) const 
{
    MicroMeterPnt const umPosition(ScrReadMicroMeterPnt(script));
    switch (nobType.GetValue())
    {
    using enum NobType::Value;
    case inputNeuron:  return make_unique<InputNeuron>(umPosition);
    case outputNeuron: return make_unique<OutputNeuron>(umPosition);
    case neuron:       return make_unique<Neuron>(umPosition);
    case knot:         return make_unique<Knot>(umPosition);
    default:
        assert(false);
        return nullptr;
    }
}

UPNob WrapCreateNob::createIoConnector(Script & script, NobType const nobType) const 
{
    vector<IoNeuron *> ioNeuronList;
    script.ScrReadSpecial(LIST_OPEN_BRACKET);
    int const iNrOfElements { script.ScrReadInt() };
    script.ScrReadSpecial(NR_SEPARATOR);
    for (int iElem { 0 };;)
    {
        NobId      const id        { ScrReadNobId(script) };
        IoNeuron * const pIoNeuron { GetWriterInterface().GetNobPtr<IoNeuron *>(id) };
        if (! pIoNeuron)
            throw ScriptErrorHandler::ScriptException(999, wstring(L"NobId not found"));
        ioNeuronList.push_back(pIoNeuron);
        if (++iElem == iNrOfElements)
            break;
        script.ScrReadSpecial(ID_SEPARATOR);
    }
    script.ScrReadSpecial(LIST_CLOSE_BRACKET);
    unique_ptr<IoConnector> upIoConnector;
    if (nobType.IsInputConnectorType())
        upIoConnector = make_unique<InputConnector> (move(ioNeuronList));
    else
        upIoConnector = make_unique<OutputConnector>(move(ioNeuronList));
    upIoConnector->AlignDirection();
    return move(upIoConnector);
}
