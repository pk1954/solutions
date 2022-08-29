// WrapSignalInfo.ixx
//
// ModelIO

module;

#include <iostream>
#include "MonitorData.h"
#include "Signal.h"

export module WrapSignalInfo;

import NNetWrapperBase;
import ErrHndl;
import SignalId;
import Script;
import NNetWrapperHelpers;
import SignalFactory;

using std::wostream;

export class WrapSignalInfo : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        MicroMeterCircle    umCircle;
        SignalId      const signalId { ScrReadSignalId(script) };
        script.ScrReadString(L"source");
        unsigned long const ulSigSrc{ script.ScrReadUlong() };
        if (ulSigSrc == Signal::SIGSRC_CIRCLE)
        {
            umCircle = ScrReadMicroMeterCircle(script);
        }
        else
        {
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101"));
        }
        SignalFactory::MakeSensorSignal(umCircle, signalId.GetTrackNr(), m_modelIO.GetImportNMWI());
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
                    out << idSignal << L" source ";
                    pSignal->WriteSignalInfo(out);
                }
            }
        );
    };
};
