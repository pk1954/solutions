// NNetModelImporter.cpp
//
// ModelIO

#include "stdafx.h"
#include <filesystem>
#include <assert.h>
#include "ERRHNDL.H"
#include "SignalFactory.h"
#include "SignalGenerator.h"
#include "UPSigGenList.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "MonitorData.h"
#include "NobException.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "IoLine.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "NNetModelStorage.h"
#include "NNetModelImporter.h"
#include "ImportTermination.h"
#include "WrapCreateNob.h"
#include "WrapVoltage.h"
#include "WrapperBase.h"

using std::filesystem::exists;
using std::make_unique;
using std::to_wstring;
using std::bit_cast;

class WrapProtocol : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        script.ScrReadString(L"version");
        double dVersion = script.ScrReadFloat();
    }
};

class WrapDescription : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        wstring const wstrDescription { script.ScrReadString() };
        GetWriterInterface().AddDescriptionLine(wstrDescription);
    }
};

class WrapGlobalParameter : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        auto const param(static_cast<ParamType::Value>(script.ScrReadUint()));
        script.ScrReadSpecial(L'=');
        float const fValue { Cast2Float(script.ScrReadFloat()) };
        GetWriterInterface().SetParam(param, fValue);
    }
};

class WrapNrOfNobs : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        script.ScrReadSpecial(L'=');
        long lNrOfNobs { script.ScrReadLong() };
        GetUPNobsRef().IncreaseSize(lNrOfNobs);
    }
};

class WrapNobParameter : public WrapperBase  // Legacy
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        script.ScrReadString(L"InputNeuron");
        NobId const id   (script.ScrReadLong());
        auto  const param(static_cast< ParamType::Value >(script.ScrReadUint()));
        assert(param == ParamType::Value::pulseRate);
        script.ScrReadSpecial(L'=');
        Cast2Float(script.ScrReadFloat()); // 
    }
};

class WrapTriggerSound : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
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

class WrapNrOfTracks : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        unsigned int const uiNrOfTracks { script.ScrReadUint() };
        for (unsigned int ui = 0; ui < uiNrOfTracks; ++ui)
            GetMonitorData().InsertTrack(TrackNr(0));
    }
};

class WrapSignal : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
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
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101"));
        }
        GetMonitorData().AddSignal(GetUPNobsRef(), trackNr, umCircle);
    }
};

class WrapSetParam : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        NobId            const id       { ScrReadNobId(script) };
        ParamType::Value const param    { ScrReadParamType(script) };
        float                  fVal     { Cast2Float(script.ScrReadFloat()) };
        if (InputConnector   * pInpConn { GetWriterInterface().GetNobPtr<InputConnector *>(id) }) // Legacy
        {                                                                                         // Legacy
            pInpConn->Apply2All                                                                   // Legacy
            (                                                                                     // Legacy
                [param, fVal](IoLine & n)                                                         // Legacy
                {                                                                                 // Legacy
                    auto & inputLine { static_cast<InputLine &>(n) };                             // Legacy
                    inputLine.GetSigGen()->SetParam(param, fVal);                                  // Legacy
                }                                                                                 // Legacy
            );                                                                                    // Legacy
        }                                                                                         // Legacy 
        else if ( InputLine * pInpNeuron { GetWriterInterface().GetNobPtr<InputLine*>(id) } )     // Legacy
        {
            pInpNeuron->GetSigGen()->SetParam(param, fVal);
        }
    }
};

class WrapSignalGenerator : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        NNetModelWriterInterface & nmwi       { GetWriterInterface() };
        wstring            const   name       { script.ScrReadString() };
        SigGenStaticData   const   sigGenData { ScrReadSigGenStaticData(script) };
        SigGenId           const   sigGenId   { nmwi.FindSigGen(name) };
        if (nmwi.IsValid(sigGenId))
        {
            SignalGenerator * pSigGen { nmwi.GetSigGen(sigGenId) };
            pSigGen->SetStaticData(sigGenData);
        }
        else
        {
            UPSigGen upSigGen { move(nmwi.NewSigGen(name)) };
            upSigGen->SetStaticData(sigGenData);
            nmwi.PushSigGen(move(upSigGen));
        }
    }
};

class WrapActiveSigGen : public WrapperBase
{
public:
    using WrapperBase::WrapperBase;

    void operator() (Script & script) const final
    {
        NNetModelWriterInterface & nmwi { GetWriterInterface() };
        SigGenId const sigGenId(script.ScrReadUint());
        if (nmwi.IsValid(sigGenId))
            nmwi.SetSigGenActive(sigGenId);
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
    SymbolTable::ScrDefConst(L"SetParam",        new WrapSetParam       (* this));
    SymbolTable::ScrDefConst(L"Voltage",         new WrapVoltage        (* this));
    SymbolTable::ScrDefConst(L"SignalGenerator", new WrapSignalGenerator(* this));
    SymbolTable::ScrDefConst(L"ActiveSigGen",    new WrapActiveSigGen   (* this));

    NobType::Apply2All
    (
        [](NobType const & type) 
        {
            SymbolTable::ScrDefConst
            (
                NobType::GetName(type.GetValue()), 
                static_cast<unsigned long>(type.GetValue()) 
            );
        }
    );

    SymbolTable::ScrDefConst(L"InputNeuron",  static_cast<unsigned long>(NobType::Value::inputLine));  // Legacy
    SymbolTable::ScrDefConst(L"OutputNeuron", static_cast<unsigned long>(NobType::Value::outputLine)); // Legacy
    SymbolTable::ScrDefConst(L"inputNeuron",  static_cast<unsigned long>(NobType::Value::inputLine));  // Legacy
    SymbolTable::ScrDefConst(L"outputNeuron", static_cast<unsigned long>(NobType::Value::outputLine)); // Legacy

    ParamType::Apply2AllParameters
    (
        [](ParamType::Value const & param) 
        {
            SymbolTable::ScrDefConst
            (
                ParamType::GetName(param), 
                static_cast<unsigned long>(param) 
            );
        }
    );
}

void NNetModelImporter::importModel() 
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
        catch (NobException const & e)
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
        m_upImportedModel.release();
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
    NNetModelImporter * pModelImporter { bit_cast<NNetModelImporter *>(data) };
    pModelImporter->importModel();
    return 0;
}

bool NNetModelImporter::Import
(
    wstring const & wstrPath,
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
    m_ImportedNMWI.SetModel(m_upImportedModel.get());
    m_wstrFile2Read = wstrPath;
    Util::RunAsAsyncThread(importModelThreadProc, static_cast<void *>(this));
    return true;
}

unique_ptr<NNetModel> NNetModelImporter::GetImportedModel() 
{ 
    return move(m_upImportedModel);
}
