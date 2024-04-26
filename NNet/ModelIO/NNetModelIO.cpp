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
import IoConstants;
import Symtab;
import Util;
import Types;
import Thread;
import ScriptFile;
import Win32_Util;
import HiResTimer;
import WrapVoltage;
import WrapProtocol;
import WrapSimulationTime;
import Wrapper;
import NobIo;
import WrapDescription;
import WrapNrOfNobs;
import WrapNrOfTracks;
import WrapActiveSigGen;
import WrapEmphasized;
import WrapScanImage;
import WrapSignalInfo;
import WrapGlobalParameter;
import WrapSetScanArea;
import WrapSignalData;
import WrapSetSigGen;
import InputOutputUI;
import NNetWrapperHelpers;
import NNetModel;
import ThreadPoolTimer;

using std::filesystem::exists;
using std::filesystem::path;
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

void NNetModelIO::Initialize(Observable * const pLockModelObservable)
{
    m_pLockModelObservable = pLockModelObservable;
    AddModelWrapper<WrapProtocol       >(L"Protocol");
    AddModelWrapper<WrapDescription    >(L"Description");
    AddModelWrapper<WrapGlobalParameter>(L"GlobalParameter");
    AddModelWrapper<WrapNrOfNobs       >(L"NrOfNobs");
    AddModelWrapper<NobIo              >(L"CreateNob");
    AddModelWrapper<WrapVoltage        >(L"Voltage");
    AddModelWrapper<WrapEmphasized     >(L"Emphasized");
    AddModelWrapper<WrapSignalGenerator>(L"SignalGenerator");
    AddModelWrapper<WrapSetSigGen      >(L"SetSigGen");
    AddModelWrapper<WrapActiveSigGen   >(L"ActiveSigGen");
    AddModelWrapper<WrapNrOfTracks     >(L"NrOfTracks");
    AddModelWrapper<WrapSignalInfo     >(L"SignalInfo");
    AddModelWrapper<WrapSignalData     >(L"SignalData");
    AddModelWrapper<WrapSetScanArea    >(L"SetScanArea");
    AddModelWrapper<WrapScanImage      >(L"ScanImage");
    AddModelWrapper<WrapSimulationTime >(L"SimulationTime");

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

void NNetModelIO::importModel() 
{
    InputOutputUI::Result res;
    Script                script;
    bool                  bSuccess { false };

    IO_STRUCT uiStruct { m_upImportUI.get(), &script };
    m_timer.StartTimer
    (
        200, // period in ms
        [](PTP_CALLBACK_INSTANCE, PVOID pContext, PTP_TIMER)
        {
            IO_STRUCT* pIoStruct { bit_cast<IO_STRUCT*>(pContext) };
            pIoStruct->pUI->ReadProgressReport(pIoStruct->pScript);
        },
        & uiStruct
    );

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
            try
            {
                CheckImportedNobId(script, m_upImportedNMWI->GetUPNobs(), e.m_id);
            }
            catch (ScriptErrorHandler::ScriptException const& errInfo)
            {
                ScriptErrorHandler::PrintErrorInfo(script.GetScanner(), errInfo);
                ScriptErrorHandler::PrintNL(L"error exit");
            }
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

    m_upImportedNMWI->PrintModelSize();
    m_timer.StopTimer();
    m_upImportUI->JobFinished(res, m_wstrFile2Read);
}

void NNetModelIO::fixProblems()
{
    m_radDislocate = 0.3_Radian;
    m_upImportedNMWI->Apply2All<Pipe>
    (
        [](Pipe& pipe)
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
    NNetModelIO::importModel();
    return 0;
}

bool NNetModelIO::Import
(
    wstring           const & wstrPath,
    unique_ptr<InputOutputUI> upInputUI
)
{
    wstring wstrNewPath { wstrPath.empty() ? m_wstrFile2Read : wstrPath };

    if (m_upImportedModel.get())
        return false;       // another import is already running

    if (! exists(wstrNewPath))
    {
        upInputUI->JobFinished(InputOutputUI::Result::fileNotFound, wstrNewPath);
        return false;
    }

    m_wstrFile2Read   = wstrNewPath;
    m_upImportUI      = move(upInputUI);
    m_upImportedNMWI  = make_unique<NNetModelWriterInterface>();
    m_upImportedModel = m_upImportedNMWI->CreateNewModel();
    ::RunAsAsyncThread(importModelThreadProc, nullptr);
    return true;
}

//////////////// export ////////////////

void NNetModelIO::writeHeader(wostream & out)
{
    out << COMMENT_SYMBOL << L" NNetModel"       << endl;
    out << COMMENT_SYMBOL << L" Created "        << GetCurrentDateAndTime() << endl;
    out << COMMENT_SYMBOL << L" Computer name: " << ::GetComputerName()       << endl;
    out << COMMENT_SYMBOL << L" User name: "     << ::GetUserName()           << endl; 
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

short NNetModelIO::GetCompactIdVal(NobId const id)
{ 
    return m_CompactIds.Get(id.GetValue()).GetValue(); 
}

size_t NNetModelIO::NrOfCompactIds() 
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
    timer.BeforeAction();
    m_pExportNMRI = & nmri;
    compress(nmri);
    wofstream modelFile;
    path modelPath { nmri.GetModelFilePath() };
    if (nmri.ModelLocked())
        modelPath.replace_extension(L"scan");
    modelFile.open(modelPath);
    writeHeader(modelFile);
    for (auto const & it : m_wrapVector)
    {
        upOutputUI->WriteProgressReport(it->GetName());
        it->Write(modelFile);
    }
    modelFile.close();
    m_pExportNMRI = nullptr;
    timer.AfterAction();
    fMicroSecs const usTilStart { PerfCounter::TicksToMicroSecs(timer.GetSingleActionTicks()) }; //for tests only
}
