// NNetModelIO.cpp
//
// ModelIO

#include <filesystem>
#include <assert.h>
#include "ErrHndl.h"
#include "Scanner.h"
#include "Signal.h"
#include "SignalFactory.h"
#include "SignalGenerator.h"
#include "UPSigGenList.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputLine.h"
#include "OutputLine.h"
#include "IoLine.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "MonitorData.h"
#include "NobException.h"
#include "ModelDescription.h"
#include "Signal.h"
#include "Track.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_thread.h"
#include "win32_hiResTimer.h"
#include "InputOutputUI.h"
#include "WrapDescription.h"
#include "WrapGlobalParameter.h"
#include "WrapNrOfNobs.h"
#include "WrapProtocol.h"
#include "WrapNobParameter.h"
#include "WrapSignal.h"
#include "WrapSignalData.h"
#include "WrapSimulationTime.h"
#include "WrapNrOfTracks.h"
#include "WrapSignalInfo.h"
#include "WrapActiveSigGen.h"
#include "WrapSetParam.h"
#include "WrapSetSigGen.h"
#include "WrapSignalGenerator.h"
#include "WrapCreateNob.h"
#include "WrapVoltage.h"
#include "WrapEEGcoordSys.h"
#include "NNetWrapperHelpers.h"
#include "NNetParameters.h"
#include "NNetModelStorage.h"
#include "NNetWrapperBase.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"

import MoreTypes;

using std::filesystem::exists;
using std::wofstream;
using std::make_unique;
using std::to_wstring;
using std::bit_cast;
using std::endl;

NNetModelIO::~NNetModelIO() = default;

void NNetModelIO::Initialize()
{
    m_wrapVector.push_back(make_unique<WrapProtocol       >(L"Protocol",        *this));
    m_wrapVector.push_back(make_unique<WrapDescription    >(L"Description",     *this));
    m_wrapVector.push_back(make_unique<WrapGlobalParameter>(L"GlobalParameter", *this));
    m_wrapVector.push_back(make_unique<WrapNrOfNobs       >(L"NrOfNobs",        *this));
    m_wrapVector.push_back(make_unique<WrapCreateNob      >(L"CreateNob",       *this));
    m_wrapVector.push_back(make_unique<WrapVoltage        >(L"Voltage",         *this));
    m_wrapVector.push_back(make_unique<WrapSignalGenerator>(L"SignalGenerator", *this));
    m_wrapVector.push_back(make_unique<WrapSetSigGen      >(L"SetSigGen",       *this));
    m_wrapVector.push_back(make_unique<WrapActiveSigGen   >(L"ActiveSigGen",    *this));
    m_wrapVector.push_back(make_unique<WrapNrOfTracks     >(L"NrOfTracks",      *this));
    m_wrapVector.push_back(make_unique<WrapSignalInfo     >(L"SignalInfo",      *this));
    m_wrapVector.push_back(make_unique<WrapSignalData     >(L"SignalData",      *this));
    m_wrapVector.push_back(make_unique<WrapSimulationTime >(L"SimulationTime",  *this));
    m_wrapVector.push_back(make_unique<WrapSetParam       >(L"SetParam",        *this)); // Legacy
    m_wrapVector.push_back(make_unique<WrapSignal         >(L"Signal",          *this)); // Legacy
    m_wrapVector.push_back(make_unique<WrapNobParameter   >(L"NobParameter",    *this)); // Legacy
//    m_wrapVector.push_back(make_unique<WrapEEGcoordSys    >(L"EEGcoordSys",     *this));

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

    SymbolTable::ScrDefConst(L"circle",       static_cast<unsigned long>(Signal::SIGSRC_CIRCLE));
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

//////////////// import ////////////////

struct IO_STRUCT  // IO thread -> timerProc
{
    InputOutputUI * pUI;
    Script        * pScript;
};

static void CALLBACK timerProc
(
    PTP_CALLBACK_INSTANCE, 
    PVOID pContext, 
    PTP_TIMER
)
{
    IO_STRUCT * pIoStruct { bit_cast<IO_STRUCT *>(pContext) };
    pIoStruct->pUI->ReadProgressReport(pIoStruct->pScript);
}

void NNetModelIO::importModel() 
{
    InputOutputUI::Result res;
    Script                script;
    bool                  bSuccess { false };

    unsigned int m_uiMsPeriod { 200 };
    FILETIME     fileTime     { m_uiMsPeriod, 0 };
    IO_STRUCT    uiStruct     { m_upImportUI.get(), &script };
    m_pTpTimer = CreateThreadpoolTimer(timerProc, &uiStruct, nullptr);
    SetThreadpoolTimer(m_pTpTimer, &fileTime, m_uiMsPeriod, m_uiMsPeriod);

    if (! m_wstrFile2Read.empty())
    {
        script.SetEcho(true);
        try
        {
            bSuccess = script.ScrProcess(m_wstrFile2Read);
        }
        catch (NobException const & e)
        {
            CheckImportedNobId(script, m_upImportedNMWI->GetUPNobs(), e.m_id);
        }
    }
    if (bSuccess)
    {
        m_upImportedNMWI->RemoveOrphans();
        m_upImportedNMWI->SetModelFilePath(m_wstrFile2Read);
        m_upImportedNMWI->DescriptionComplete();
        res = InputOutputUI::Result::ok;
    }
    else
    {
        m_upImportedModel.release();
        res = InputOutputUI::Result::errorInFile;
    }
    if (m_pTpTimer)
        CloseThreadpoolTimer(m_pTpTimer);
    m_upImportUI->JobFinished(res, m_wstrFile2Read);
}

void NNetModelIO::CheckImportedNobId
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
    NNetModelIO * pModelIO { bit_cast<NNetModelIO *>(data) };
    pModelIO->importModel();
    return 0;
}

bool NNetModelIO::Import
(
    wstring           const & wstrPath,
    unique_ptr<InputOutputUI> upInputUI
)
{
    if (wstrPath.empty())
        return false;

    if (m_upImportedModel.get()) 
        return false;       // another import is already running

    if (! exists(wstrPath))
    {
        upInputUI->JobFinished(InputOutputUI::Result::fileNotFound, wstrPath);
        return false;
    }

    m_upImportUI      = move(upInputUI);
    m_upImportedModel = make_unique<NNetModel>(); // do not initialize here
    m_upImportedNMWI  = make_unique<NNetModelWriterInterface>();
    m_upImportedNMWI->SetModel(m_upImportedModel.get());
    m_wstrFile2Read = wstrPath;
    Util::RunAsAsyncThread(importModelThreadProc, static_cast<void *>(this));
    return true;
}

unique_ptr<NNetModel> NNetModelIO::GetImportedModel() 
{ 
    m_upImportedNMWI.release();
    return move(m_upImportedModel);
}

//////////////// export ////////////////

void NNetModelIO::writeHeader(wostream & out) const
{
    out << Scanner::COMMENT_SYMBOL << L" NNetModel"       << endl;
    out << Scanner::COMMENT_SYMBOL << L" Created "        << Util::GetCurrentDateAndTime() << endl;
    out << Scanner::COMMENT_SYMBOL << L" Computer name: " << Util::GetComputerName()       << endl;
    out << Scanner::COMMENT_SYMBOL << L" User name: "     << Util::GetUserName()           << endl; 
    out << endl;
}

void NNetModelIO::compress(NNetModelReaderInterface const & nmri)
{
    m_CompactIds.Resize(nmri.GetSizeOfNobList());
    NobId idCompact(0);
    for (int i = 0; i < m_CompactIds.Size(); ++i)
    {
        m_CompactIds.SetAt(i, nmri.GetConstNob(NobId(i)) ? idCompact++ : NobId());
    }
}

int NNetModelIO::GetCompactIdVal(NobId const id) const
{ 
    return m_CompactIds.Get(id.GetValue()).GetValue(); 
}

size_t NNetModelIO::NrOfCompactIds() const 
{ 
    return m_CompactIds.Size(); 
}

void NNetModelIO::Export
(
    NNetModelReaderInterface const & nmri,
    unique_ptr<InputOutputUI>        upOutputUI
)
{
    HiResTimer timer;
    timer.Start();

    m_pExportNMRI = & nmri;
    compress(nmri);
    wofstream modelFile(nmri.GetModelFilePath());
    writeHeader(modelFile);
    for (auto const & it : m_wrapVector)
    {
        upOutputUI->WriteProgressReport(it->GetName());
        it->Write(modelFile);
    }
    modelFile.close();
    m_pExportNMRI = nullptr;

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart() }; //for tests only
}
