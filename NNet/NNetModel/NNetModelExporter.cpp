// NNetModelExporter.cpp
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <assert.h>
#include <filesystem>
#include "Signal.h"
#include "Track.h"
#include "InputNeuron.h"
#include "BaseKnot.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_HiResTimer.h"
#include "NNetModelStorage.h"
#include "NNetModelExporter.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::path;

void NNetModelExporter::Initialize(NNetModelReaderInterface * const pNMRI)
{
    m_pNMRI = pNMRI;
}

size_t const NNetModelExporter::getCompactIdVal(NobId const id) 
{ 
    return m_CompactIds.Get(id.GetValue()).GetValue(); 
}

void NNetModelExporter::writeHeader(wostream & out)
{
    static int const BUF_SIZE { 128 };

    out << Scanner::COMMENT_SYMBOL << L" NNetModel" << endl;
    out << Scanner::COMMENT_SYMBOL << L" Created " << Util::GetCurrentDateAndTime() << endl;

    WCHAR  infoBuf[BUF_SIZE];
    DWORD  bufCharCount = BUF_SIZE;

    // Get and display the name of the computer.
    GetComputerName(infoBuf, &bufCharCount);
    out << Scanner::COMMENT_SYMBOL << L" Computer name: " << infoBuf << endl;

    // Get and display the user name.
    GetUserName(infoBuf, &bufCharCount);
    out << Scanner::COMMENT_SYMBOL << L" User name: " << infoBuf << endl; 
    out << endl;

    out << L"Protocol version " << NNetModelStorage::PROTOCOL_VERSION << endl;
    out << endl;
}

void NNetModelExporter::writeGlobalParameters(wostream & out)
{
    ParamType::Apply2GlobalParameters
    (
        [&](ParamType::Value const & par) 
        {
            out << L"GlobalParameter " << ParamType::GetName(par) << L" = "
                << m_pNMRI->GetParameter(par) 
                << endl; 
        }
   );
}

void NNetModelExporter::writeNobs(wostream & out)
{
    m_CompactIds.Resize(m_pNMRI->GetSizeOfNobList());
    NobId idCompact(0);
    for (int i = 0; i < m_CompactIds.Size(); ++i)
    {
        m_CompactIds.SetAt
        (
            i, 
            m_pNMRI->GetConstNob(NobId(i)) ? idCompact++ : NobId()
       );
    }
    out << L"NrOfNobs = " << idCompact << endl;
    out << endl;
    m_pNMRI->GetUPNobs().Apply2All<BaseKnot   >([&](BaseKnot    const & s) { writeNob(out, s); });
    m_pNMRI->GetUPNobs().Apply2All<Pipe       >([&](Pipe        const & s) { writeNob(out, s); });
    m_pNMRI->GetUPNobs().Apply2All<IoConnector>([&](IoConnector const & s) { writeNob(out, s); });
}

void NNetModelExporter::writeNobParameters(wostream & out)   // Legacy
{
    m_pNMRI->GetUPNobs().Apply2All<InputConnector>
    (
        [&](InputConnector const & inpConn)
        { 
            NobId           const   id     { getCompactIdVal(inpConn.GetId()) };
            SignalGenerator const & sigGen { inpConn.GetSignalGenerator() };
            out << L"SetParam" << id << ParamType::Value::baseFrequency   << sigGen.FreqBase() << endl; 
            out << L"SetParam" << id << ParamType::Value::stimulusMaxFreq << sigGen.FreqMax()  << endl; 
            out << L"SetParam" << id << ParamType::Value::stimulusMaxTime << sigGen.TimeMax()  << endl; 
        }
   );
}

void NNetModelExporter::writeTriggerSounds(wostream & out)
{
    m_pNMRI->GetUPNobs().Apply2All<Neuron>
    (
        [&](Neuron const & neuron) 
        { 
            if (neuron.HasTriggerSound())
            {
                SoundDescr sound { neuron.GetTriggerSound() };
                out << L"TriggerSound " << getCompactIdVal(neuron.GetId()) << L" "
                    << sound.m_frequency << L" Hertz "
                    << sound.m_duration  << L" msec "
                    << endl; 
            }
        } 
   );
}

void NNetModelExporter::writeMonitorData(wostream & out)
{
    MonitorData const & monitorData { m_pNMRI->GetMonitorData() };

    out << L"NrOfTracks " << monitorData.GetNrOfTracks() << endl;

    monitorData.Apply2AllSignals
    (
        [&](SignalId const idSignal)
        {
            Signal const * const pSignal { monitorData.GetSignalPtr(idSignal) };
            out << L"Signal "; 
            WriteTrackNr(out, idSignal.GetTrackNr());
            out << L" source " << NNetModelStorage::SIGSRC_CIRCLE; 
            if (pSignal)
                out << pSignal->GetCircle();
            out << endl; 
        }
   );
}

void NNetModelExporter::writeDescription(wostream & out)
{
    wstring wstrLine;
    int iLineNr = 0;
    while (m_pNMRI->GetDescriptionLine(iLineNr++, wstrLine))
    {
        out << L"Description \"" << wstrLine << "\"" << endl;
    }
}

void NNetModelExporter::writePipe(wostream & out, Pipe const & pipe)
{
    out << Pipe::OPEN_BRACKET 
        << getCompactIdVal(pipe.GetStartKnotId()) 
        << Pipe::SEPARATOR
        << getCompactIdVal(pipe.GetEndKnotId()) 
        << Pipe::CLOSE_BRACKET;
}

void NNetModelExporter::writeIoConnector(wostream & out, IoConnector const & conn)
{
    assert(conn.Size() > 0);
    out << BaseKnot::OPEN_BRACKET << conn.Size() << BaseKnot::NR_SEPARATOR;
    for (size_t i = 0; i < conn.Size() - 1; ++i)
    {
        out << getCompactIdVal(conn.GetElem(i).GetId());
        out << BaseKnot::ID_SEPARATOR;
    }
    out << getCompactIdVal(conn.GetElem(conn.Size()-1).GetId());
    out << BaseKnot::CLOSE_BRACKET;
}

void NNetModelExporter::writeNob(wostream & out, Nob const & nob)
{
    if (nob.IsDefined())
    {
        out << L"CreateNob " << getCompactIdVal(nob.GetId()) << L" " << nob.GetName();
        switch (nob.GetNobType().GetValue())
        {
        case NobType::Value::inputNeuron:
        case NobType::Value::outputNeuron:
        case NobType::Value::neuron:
        case NobType::Value::knot:
            out << static_cast<BaseKnot const &>(nob).GetPos();
            break;

        case NobType::Value::pipe:
            writePipe(out, static_cast<Pipe const &>(nob));
            break;

        case NobType::Value::inputConnector:
        case NobType::Value::outputConnector:
            writeIoConnector(out, static_cast<IoConnector const &>(nob));
            break;

        default:
            assert(false);
            break;
        }
        out << endl;
    }
}

void NNetModelExporter::write(wostream & out)
{
    HiResTimer timer;
    timer.Start();

    writeHeader(out);
    out << endl;
    writeDescription(out);
    out << endl;
    writeGlobalParameters(out);
    out << endl;
    writeNobs(out);
    out << endl;
    writeNobParameters(out);
    out << endl;
    writeTriggerSounds(out);
    out << endl;
    writeMonitorData(out);
    out << endl;

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart() }; //for tests only
}

void NNetModelExporter::WriteModel()
{
    wofstream modelFile(m_pNMRI->GetModelFilePath());
    write(modelFile);
    modelFile.close();
}
