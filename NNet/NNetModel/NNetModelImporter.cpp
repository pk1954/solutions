// NNetModelImporter.cpp
//
// NNetModel

#include "stdafx.h"
#include <filesystem>
#include <assert.h>
#include "ErrHndl.h"
#include "SignalFactory.h"
#include "Knot.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "MonitorData.h"
#include "NobException.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "IoNeuron.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "NNetModelStorage.h"
#include "NNetModelImporter.h"

using std::filesystem::exists;
using std::make_unique;
using std::to_wstring;

class WrapBase : public ScriptFunctor
{
public:
    WrapBase(NNetModelImporter & modelImporter) :
        m_modelImporter(modelImporter)
    { };

protected:
    NNetModelWriterInterface & GetWriterInterface() const { return m_modelImporter.getWriterInterface(); }
    MonitorData              & GetMonitorData()     const { return m_modelImporter.getWriterInterface().GetMonitorData(); }
    UPNobList                & GetUPNobsRef()       const { return m_modelImporter.getWriterInterface().GetUPNobs(); }

    NNetModelImporter & m_modelImporter;
};

class WrapProtocol : public WrapBase
{
public:
    WrapProtocol(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {
        script.ScrReadString(L"version");
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public WrapBase
{
public:
    WrapDescription(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {
        wstring const wstrDescription { script.ScrReadString() };
        GetWriterInterface().AddDescriptionLine(wstrDescription);
    }
};

class WrapCreateNob : public WrapBase
{
public:
    WrapCreateNob(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {   
        createNob(script);
    }

private:

    Nob * const createNob(Script & script) const
    {   
        NobId   const idFromScript { ScrReadNobId(script) };
        NobType const nobType      { ScrReadNobType(script) };
        Nob         * pNob         { nullptr };
        UPNob         upNob        {};   
        { 
            switch (nobType.GetValue())
            {
            case NobType::Value::inputConnector:
            case NobType::Value::outputConnector:
                 upNob = createIoConnector(script, nobType);
                 break;
            case NobType::Value::inputNeuron:
            case NobType::Value::outputNeuron:
            case NobType::Value::neuron:
            case NobType::Value::knot:
                 upNob = createBaseKnot(script, nobType);
                 break;
            case NobType::Value::pipe:
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
        if (nobType.IsIoConnectorType())
        {
            static_cast<IoConnector *>(pNob)->SetParentPointers();
        }
        return pNob;
    }

    UPNob createPipe(Script & script) const
    {
        script.ScrReadSpecial(Pipe::OPEN_BRACKET);
        NobId const idStart { ScrReadNobId(script) };
        for (int i = 0; i < Pipe::SEPARATOR.length(); i++)
            script.ScrReadSpecial(Pipe::SEPARATOR[i]);        
        NobId const idEnd { ScrReadNobId(script) };
        script.ScrReadSpecial(Pipe::CLOSE_BRACKET);
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
            unique_ptr<Pipe> upPipe { make_unique<Pipe>(pKnotStart, pKnotEnd) };
            pKnotStart->AddOutgoing(upPipe.get());
            pKnotEnd  ->AddIncoming(upPipe.get());
            return move(upPipe);
        }
    }

    UPNob createBaseKnot(Script & script, NobType const nobType) const 
    {
        MicroMeterPnt const umPosition(ScrReadMicroMeterPnt(script));
        switch (nobType.GetValue())
        {
        case NobType::Value::inputNeuron:
            return make_unique<InputNeuron>(umPosition);

        case NobType::Value::outputNeuron:
            return make_unique<OutputNeuron>(umPosition);

        case NobType::Value::neuron:
            return make_unique<Neuron>(umPosition);

        case NobType::Value::knot:
            return make_unique<Knot>(umPosition);

        default:
            assert(false);
            return nullptr;
        }
    }

    UPNob createIoConnector(Script & script, NobType const nobType) const 
    {
        vector<IoNeuron *> ioNeuronList;
        script.ScrReadSpecial(BaseKnot::OPEN_BRACKET);
        int const iNrOfElements { script.ScrReadInt() };
        script.ScrReadSpecial(BaseKnot::NR_SEPARATOR);
        for (int iElem { 0 };;)
        {
            NobId      const id        { ScrReadNobId(script) };
            IoNeuron * const pIoNeuron { GetWriterInterface().GetNobPtr<IoNeuron *>(id) };
            if (! pIoNeuron)
                throw ScriptErrorHandler::ScriptException(999, wstring(L"NobId not found"));
            ioNeuronList.push_back(pIoNeuron);
            if (++iElem == iNrOfElements)
                break;
            script.ScrReadSpecial(BaseKnot::ID_SEPARATOR);
        }
        script.ScrReadSpecial(BaseKnot::CLOSE_BRACKET);
        unique_ptr<IoConnector> upIoConnector;
        if (nobType.IsInputConnectorType())
            upIoConnector = make_unique<InputConnector> (move(ioNeuronList));
        else
            upIoConnector = make_unique<OutputConnector>(move(ioNeuronList));
        upIoConnector->AlignDirection();
        return move(upIoConnector);
    }
};

class WrapGlobalParameter : public WrapBase
{
public:
    WrapGlobalParameter(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {
        ParamType::Value const param(static_cast<ParamType::Value>(script.ScrReadUint()));
        script.ScrReadSpecial(L'=');
        float const fValue { Cast2Float(script.ScrReadFloat()) };
        GetWriterInterface().SetParam(param, fValue);
    }
};

class WrapNrOfNobs : public WrapBase
{
public:
    WrapNrOfNobs(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {
        script.ScrReadSpecial(L'=');
        long lNrOfNobs { script.ScrReadLong() };
        GetUPNobsRef().IncreaseSize(lNrOfNobs);
    }
};

class WrapNobParameter : public WrapBase
{
public:
    WrapNobParameter(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const
    {
        script.ScrReadString(L"InputNeuron");
        NobId            const id   (script.ScrReadLong());
        ParamType::Value const param(static_cast< ParamType::Value >(script.ScrReadUint()));
        assert(param == ParamType::Value::pulseRate);
        script.ScrReadSpecial(L'=');
        float const fValue { Cast2Float(script.ScrReadFloat()) };
        GetWriterInterface().GetNobPtr<InputNeuron *>(id)->SetBaseFrequency(fHertz(fValue));
    }
};

class WrapTriggerSound : public WrapBase
{
public:
    WrapTriggerSound(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const  
    {
        NobId const id      { ScrReadNobId(script) };
        Neuron    * pNeuron { GetWriterInterface().GetNobPtr<Neuron *>(id) };
        Hertz const freq    { script.ScrReadUlong() };
        script.ScrReadString(L"Hertz");
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString(L"msec");
        pNeuron->SetTriggerSound(SoundDescr{ true, freq, msec });
    }
};

class WrapNrOfTracks : public WrapBase
{
public:
    WrapNrOfTracks(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const 
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        for (unsigned int ui = 0; ui < uiNrOfTracks; ++ui)
            GetMonitorData().InsertTrack(TrackNr(0));
    }
};

class WrapSignal : public WrapBase
{
public:
    WrapSignal(NNetModelImporter & m) : WrapBase(m) { };

    virtual void operator() (Script & script) const 
    {
        MicroMeterCircle umCircle;
        TrackNr          trackNr { ScrReadTrackNr(script) };
        script.ScrReadString(L"source");
        unsigned long    ulSigSrc { script.ScrReadUlong() };
        if (ulSigSrc == NNetModelStorage::SIGSRC_CIRCLE)
        {
            umCircle = ScrReadMicroMeterCircle(script);
        }
        else
        {
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101") );
        }
        GetMonitorData().AddSignal(trackNr, umCircle);
    }
};

void NNetModelImporter::Initialize()
{
    SymbolTable::ScrDefConst(L"circle",          NNetModelStorage::SIGSRC_CIRCLE );
    SymbolTable::ScrDefConst(L"Description",     new WrapDescription    (* this));
    SymbolTable::ScrDefConst(L"Protocol",        new WrapProtocol       (* this));
    SymbolTable::ScrDefConst(L"GlobalParameter", new WrapGlobalParameter(* this));
    SymbolTable::ScrDefConst(L"NrOfNobs",        new WrapNrOfNobs       (* this));
    SymbolTable::ScrDefConst(L"CreateNob",       new WrapCreateNob      (* this));
    SymbolTable::ScrDefConst(L"NobParameter",    new WrapNobParameter   (* this));
    SymbolTable::ScrDefConst(L"TriggerSound",    new WrapTriggerSound   (* this));
    SymbolTable::ScrDefConst(L"NrOfTracks",      new WrapNrOfTracks     (* this));
    SymbolTable::ScrDefConst(L"Signal",          new WrapSignal         (* this));

    NobType::Apply2All
    (
        [&](NobType const & type) 
        {
            SymbolTable::ScrDefConst
            (
                NobType::GetName(type.GetValue()), 
                static_cast<unsigned long>(type.GetValue()) 
           );
        }
   );

    ParamType::Apply2AllParameters
    (
        [&](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst
            (
                ParamType::GetName(param), 
                static_cast<unsigned long>(param) 
           );
        }
   );
}

void NNetModelImporter::import() 
{
    ImportTermination::Result res;
    Script                    script;
    bool                      bSuccess { false };

    if (! m_wstrFile2Read.empty())
    {
        script.SetEcho(true);
        try
        {
            bSuccess = script.ScrProcess(m_wstrFile2Read);
        }
        catch (NobException e)
        {
            CheckImportedNobId(script, m_ImportedNMWI.GetUPNobs(), e.m_id);
        }
    }
    if (bSuccess)
    {
        m_ImportedNMWI.RemoveOrphans();
        m_ImportedNMWI.SetModelFilePath(m_wstrFile2Read);
        m_ImportedNMWI.DescriptionComplete();
        res = ImportTermination::Result::ok;
    }
    else
    {
        res = ImportTermination::Result::errorInFile;
    }
    m_upTermination->Reaction(res, m_wstrFile2Read);
}

void NNetModelImporter::CheckImportedNobId
(
    Script          & script, 
    UPNobList const & list,
    NobId     const   id 
)
{        
    wstring const strNobId { to_wstring(id.GetValue()) };
    if (IsUndefined(id))
    {
        script.SetExpectedToken(L"NobId != NO_NOB");
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
    }
    else if (! list.IsValidNobId(id))
    {
        script.SetExpectedToken(L"id < " + to_wstring(list.Size()));
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
    }
    else if (! list.IsNobDefined(id))
    {
        script.SetExpectedToken(L"Defined NobId");
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Nob is not defined: ") + strNobId);
    }
};

static unsigned int __stdcall importModelThreadProc(void * data) 
{
    SetThreadDescription(GetCurrentThread(), L"ImportModel");
    NNetModelImporter * pModelImporter { reinterpret_cast<NNetModelImporter *>(data) };
    pModelImporter->import();
    return 0;
}

bool NNetModelImporter::Import
(
    wstring                 const wstrPath,
    unique_ptr<ImportTermination> upTermination
)
{
    if (wstrPath.empty())
        return false;

    if (m_upImportedModel.get()) 
        return false;       // another import is already running

    if (! exists(wstrPath))
    {
        upTermination->Reaction(ImportTermination::Result::fileNotFound, wstrPath);
        return false;
    }

    m_upTermination   = move(upTermination);
    m_upImportedModel = make_unique<NNetModel>(); // do not initialize here
    m_ImportedNMWI.Start(m_upImportedModel.get());
    m_wstrFile2Read = wstrPath;
    Util::RunAsAsyncThread(importModelThreadProc, static_cast<void *>(this));
    return true;
}

unique_ptr<NNetModel> NNetModelImporter::GetImportedModel() 
{ 
    return move(m_upImportedModel);
}

UPNobList NNetModelImporter::GetUPNobs()
{ 
    return move(m_upImportedModel->GetUPNobs()); 
}
