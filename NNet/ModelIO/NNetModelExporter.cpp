// NNetModelExporter.cpp
//
// ModelIO

#include "stdafx.h"
#include "SCANNER.H"
#include <assert.h>
#include <filesystem>
#include "Signal.h"
#include "Track.h"
#include "Neuron.h"
#include "InputLine.h"
#include "BaseKnot.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_hiResTimer.h"
#include "NNetModelStorage.h"
#include "NNetModelExporter.h"

using std::wcout;
using std::endl;
using std::put_time;
using std::wofstream;
using std::filesystem::path;

int NNetModelExporter::getCompactIdVal(NobId const id) const
{ 
    return m_CompactIds.Get(id.GetValue()).GetValue(); 
}

void NNetModelExporter::writeHeader(wostream & out) const
{
    out << Scanner::COMMENT_SYMBOL << L" NNetModel" << endl;
    out << Scanner::COMMENT_SYMBOL << L" Created "        << Util::GetCurrentDateAndTime() << endl;
    out << Scanner::COMMENT_SYMBOL << L" Computer name: " << Util::GetComputerName()       << endl;
    out << Scanner::COMMENT_SYMBOL << L" User name: "     << Util::GetUserName()           << endl; 
    out << endl;

    out << L"Protocol version " << NNetModelStorage::PROTOCOL_VERSION << endl;
    out << endl;
}

void NNetModelExporter::writeGlobalParameters(wostream & out) const
{
    ParamType::Apply2GlobalParameters
    (
        [this, &out](ParamType::Value const & par) 
        {
            out << L"GlobalParameter" << par << L" = "
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
        m_CompactIds.SetAt(i, m_pNMRI->GetConstNob(NobId(i)) ? idCompact++ : NobId());
    }
    out << L"NrOfNobs = " << idCompact << endl;
    out << endl;
    m_pNMRI->Apply2AllC<BaseKnot   >([this, &out](BaseKnot    const & s) { writeNob(out, s); });
    m_pNMRI->Apply2AllC<Pipe       >([this, &out](Pipe        const & s) { writeNob(out, s); });
    m_pNMRI->Apply2AllC<IoConnector>([this, &out](IoConnector const & s) { writeNob(out, s); });
}

void NNetModelExporter::writeSigGenStaticData(wostream & out) const
{
    m_pNMRI->GetSigGenList().Apply2AllC
    (
        [this, &out](auto const & upSigGen)
        { 
            out << L"SignalGenerator \"" 
                << upSigGen->GetName() 
                << "\" " 
                << upSigGen->GetStaticData() 
                << endl;
        }
   );
}

void NNetModelExporter::writeTriggerSounds(wostream & out) const
{
    m_pNMRI->Apply2AllC<Neuron>
    (
        [this, &out](Neuron const & neuron) 
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

void NNetModelExporter::writeMonitorData(wostream & out) const
{
    MonitorData const & monitorData { m_pNMRI->GetConstMonitorData() };

    out << L"NrOfTracks " << monitorData.GetNrOfTracks() << endl;

    monitorData.Apply2AllSignalIdsC
    (
        [&out, &monitorData](SignalId const idSignal)
        {
            Signal const * const pSignal { monitorData.GetConstSignalPtr(idSignal) };
            out << L"Signal "; 
            WriteTrackNr(out, idSignal.GetTrackNr());
            out << L" source " << NNetModelStorage::SIGSRC_CIRCLE; 
            if (pSignal)
                out << pSignal->GetCircle();
            out << endl; 
        }
   );
}

void NNetModelExporter::writeDescription(wostream & out) const
{
    wstring wstrLine;
    int iLineNr = 0;
    while (m_pNMRI->GetDescriptionLine(iLineNr++, wstrLine))
    {
        out << L"Description \"" << wstrLine << "\"" << endl;
    }
}

void NNetModelExporter::writePipe(wostream & out, Pipe const & pipe) const
{
    out << OPEN_BRACKET 
        << getCompactIdVal(pipe.GetStartKnotId()) 
        << FROM_TO
        << getCompactIdVal(pipe.GetEndKnotId()) 
        << CLOSE_BRACKET;
}

void NNetModelExporter::writePipeVoltage(wostream & out, Pipe const & pipe) const
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

void NNetModelExporter::writeNob(wostream & out, Nob const & nob) const
{
    using enum NobType::Value;
    if (nob.IsDefined())
    {
        out << L"CreateNob " << getCompactIdVal(nob.GetId()) << L" " << nob.GetName();
        switch (nob.GetNobType().GetValue())
        {
        case inputLine:
        case outputLine:
        case neuron:
        case knot:
            out << static_cast<BaseKnot const &>(nob).GetPos();
            break;

        case pipe:
            writePipe(out, static_cast<Pipe const &>(nob));
            break;

        case inputConnector:
        case outputConnector:
            out << static_cast<IoConnector const &>(nob);
            break;

        default:
            assert(false);
            break;
        }
        out << endl;

        out << L"Voltage " << getCompactIdVal(nob.GetId()) << L" " << nob.GetName();
        switch (nob.GetNobType().GetValue())
        {
        case inputLine:
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
    writeSigGenStaticData(out);
    out << endl;
    writeTriggerSounds(out);
    out << endl;
    writeMonitorData(out);
    out << endl;

    timer.Stop();
    fMicroSecs const usTilStart { timer.GetMicroSecsTilStart() }; //for tests only
}

void NNetModelExporter::WriteModel(NNetModelReaderInterface const & nmri)
{
    m_pNMRI = &nmri;
    wofstream modelFile(m_pNMRI->GetModelFilePath());
    write(modelFile);
    modelFile.close();
}
