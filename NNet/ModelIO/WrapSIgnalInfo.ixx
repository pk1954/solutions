// WrapSignalInfo.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapSignalInfo;

import NNetWrapperBase;
import ErrHndl;
import Script;
import Types;
import NNetWrapperHelpers;
import NNetModel;

using std::wostream;
using std::wstring;

export class WrapSignalInfo : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    inline static wstring const SOURCE { L"source" };

    void operator() (Script& script) const final
    {
        SignalId const signalId { ScrReadSignalId(script) };
        script.ScrReadString(SOURCE);
        unsigned long const ulSigSrc { script.ScrReadUlong() };
        if (ulSigSrc == Signal::SIGSRC_CIRCLE)
        {
            MicroMeterCircle umCircle = ScrReadMicroMeterCircle(script);
            SignalFactory::MakeSensorSignal(umCircle, signalId.GetTrackNr(), m_modelIO.GetImportNMWI());
        }
        else if (ulSigSrc == Signal::SIGSRC_NOB)
        {
            NobId nobId { ScrReadNobId(script) };
            if (IsDefined(nobId))
                SignalFactory::MakeMicroSensorSignal(nobId, signalId.GetTrackNr(), m_modelIO.GetImportNMWI());
        }
        else
        {
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101"));
        }
    }

    void Write(wostream& out) const final
    {
        MonitorData const& monitorData{ m_modelIO.GetExportNMRI().GetMonitorDataC() };
        monitorData.Apply2AllSignalIdsC
        (
            [this, &out, &monitorData](SignalId const idSignal)
            {
                if (Signal const* const pSignal{ monitorData.GetConstSignalPtr(idSignal) })
                {
                    WriteCmdName(out);
                    out << idSignal << L' ' << SOURCE << L' ';
                    pSignal->WriteSignalInfo(out);
                }
            }
        );
    };
};
