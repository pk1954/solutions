// NNetModelIO.cpp
//
// ModelIO

module;

#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <memory>
#include <Windows.h>

module NNetModelIO;

import NNetModelStorage;
import WrapSignalGenerator;
import Observable;
import ErrHndl;
import Scanner;
import Symtab;
import Types;
import Thread;
import ScriptFile;
import Win32_Util;
import HiResTimer;
import WrapVoltage;
import WrapProtocol;
import WrapSimulationTime;
import NNetWrapperBase;
import NobIo;
import WrapDescription;
import WrapNrOfNobs;
import WrapNrOfTracks;
import WrapActiveSigGen;
import WrapEmphasized;
import WrapSignalInfo;
import WrapGlobalParameter;
import WrapSignalData;
import WrapSetSigGen;
import InputOutputUI;
import NNetWrapperHelpers;
import NNetModel;

using std::filesystem::exists;
using std::wofstream;
using std::wostream;
using std::wostringstream;
using std::make_unique;
using std::unique_ptr;
using std::to_wstring;
using std::wstring;
using std::bit_cast;
using std::endl;
using std::setprecision;

NNetModelIO::~NNetModelIO() = default;

void NNetModelIO::Initialize()
{
    m_wrapVector.push_back(make_unique<WrapProtocol       >(L"Protocol",        *this));
    m_wrapVector.push_back(make_unique<WrapDescription    >(L"Description",     *this));
    m_wrapVector.push_back(make_unique<WrapGlobalParameter>(L"GlobalParameter", *this));
    m_wrapVector.push_back(make_unique<WrapNrOfNobs       >(L"NrOfNobs",        *this));
    m_wrapVector.push_back(make_unique<NobIo              >(L"CreateNob",       *this));
    m_wrapVector.push_back(make_unique<WrapVoltage        >(L"Voltage",         *this));
    m_wrapVector.push_back(make_unique<WrapEmphasized     >(L"Emphasized",      *this));
    m_wrapVector.push_back(make_unique<WrapSignalGenerator>(L"SignalGenerator", *this));
    m_wrapVector.push_back(make_unique<WrapSetSigGen      >(L"SetSigGen",       *this));
    m_wrapVector.push_back(make_unique<WrapActiveSigGen   >(L"ActiveSigGen",    *this));
    m_wrapVector.push_back(make_unique<WrapNrOfTracks     >(L"NrOfTracks",      *this));
    m_wrapVector.push_back(make_unique<WrapSignalInfo     >(L"SignalInfo",      *this));
    m_wrapVector.push_back(make_unique<WrapSignalData     >(L"SignalData",      *this));
    m_wrapVector.push_back(make_unique<WrapSimulationTime >(L"SimulationTime",  *this));

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

    SymbolTable::ScrDefConst(L"circle", static_cast<unsigned long>(Signal::SIGSRC_CIRCLE));
    SymbolTable::ScrDefConst(L"nob",    static_cast<unsigned long>(Signal::SIGSRC_NOB));

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

    SymbolTable::ScrDefConst(L"SpikeWidth",  static_cast<unsigned long>(ParamType::Value::pulseWidth));     // legacy
    SymbolTable::ScrDefConst(L"PeakVoltage", static_cast<unsigned long>(ParamType::Value::neuronPeakVolt)); // legacy
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
        catch (ProtocollException const& e)
        {
            wostringstream wBuffer;
            wBuffer << L"Cannot read model file with old protocol version " << e.m_dVersion 
                    << endl
                    << L"This program needs at least protocol version " 
                    << setprecision(2) << NNetModelStorage::PROTOCOL_VERSION 
                    << endl;
            MessageBox
            (
                nullptr,
                wBuffer.str().c_str(),
                L"Error reading model file",
                MB_OK
            );
        }
        catch (NobException const& e)
        {
            CheckImportedNobId(script, m_upImportedNMWI->GetUPNobs(), e.m_id);
        }
    }
    if (bSuccess)
    {
        fixProblems();
        m_upImportedNMWI->Apply2All<PosNob>([](PosNob& p) { p.Recalc(); });
        m_upImportedNMWI->Apply2All<Pipe>  ([](Pipe  & p) { p.RecalcSegments(); });
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

void NNetModelIO::fixProblems()
{
    m_radDislocate = 0.3_Radian;
    m_upImportedNMWI->Apply2All<Pipe>
    (
        [this](Pipe& pipe)
        {
            if ((pipe.GetEndPoint() - pipe.GetStartPoint()).IsCloseToZero())
            {
                dislocate(*Cast2PosNob(pipe.GetStartNobPtr()));
                dislocate(*Cast2PosNob(pipe.GetEndNobPtr()));
            }
        }
    );
}

void NNetModelIO::dislocate(PosNob& posNob)
{
    MicroMeterPnt umPnt { posNob.GetPos() };
    MicroMeterPnt umPntDisloc { umPnt * 0.001f };
    m_radDislocate += 0.1_Radian;
    Normalize(m_radDislocate);
    umPntDisloc.Rotate(NP_NULL, m_radDislocate);
    umPnt += umPntDisloc;
    posNob.SetPosNoFix(umPnt);
}

void NNetModelIO::CheckImportedNobId
(
    Script          & script, 
    UPNobList const & list,
    NobId     const   id 
)
{        
    if (IsUndefined(id))
    {
        wstring const strNobId { to_wstring(id.GetValue()) };
        script.SetExpectedToken(L"NobId != NO_NOB");
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
    }
    else if (! list.IsValidNobId(id))
    {
        wstring const strNobId { to_wstring(id.GetValue()) };
        script.SetExpectedToken(L"id < " + to_wstring(list.Size()));
        throw ScriptErrorHandler::ScriptException(999, wstring(L"Invalid nob id: ") + strNobId);
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
    m_upImportedNMWI  = make_unique<NNetModelWriterInterface>();
    m_upImportedModel = m_upImportedNMWI->CreateNewModel();
    m_wstrFile2Read   = wstrPath;
    Util::RunAsAsyncThread(importModelThreadProc, static_cast<void *>(this));
    return true;
}

unique_ptr<Model> NNetModelIO::GetImportedModel() 
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
    int iCount { 0 };
    for (int i = 0; i < m_CompactIds.Size(); ++i)
        m_CompactIds.SetAt(i, nmri.GetConstNob(NobId(i)) ? NobId(iCount++) : NO_NOB);
    m_nrOfcompactIds = iCount;
}

int NNetModelIO::GetCompactIdVal(NobId const id) const
{ 
    return m_CompactIds.Get(id.GetValue()).GetValue(); 
}

size_t NNetModelIO::NrOfCompactIds() const 
{
    return m_nrOfcompactIds;
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
    wofstream modelFile;
    modelFile.open(nmri.GetModelFilePath());
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
