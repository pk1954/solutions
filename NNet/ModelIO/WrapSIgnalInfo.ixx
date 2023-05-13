// WrapSignalInfo.ixx
//
// ModelIO

module;

#include <cassert>
#include <string>
#include <iostream>

export module WrapSignalInfo;

import NNetWrapperBase;
import ErrHndl;
import Script;
import Types;
import IoUtil;
import NNetWrapperHelpers;
import NNetModel;

using std::wostream;
using std::wstring;
using std::endl;

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
                    SignalSource const* pSigSrc { pSignal->GetSignalSource() };
                    SignalSource::Type  type    { pSigSrc->SignalSourceType()  };
                    if (type == SignalSource::Type::macroSensor)
                    {
                        static_cast<Sensor const *>(pSigSrc)->WriteInfo(out);
                    }
                    else if (type == SignalSource::Type::microSensor)
                    {
                        MicroSensor const& microSensor  { static_cast<MicroSensor const&>(*pSigSrc) };
                        NobId       const  idNob        { microSensor.GetNobId() };
                        NobId       const  idNobCompact { m_modelIO.GetCompactIdVal(idNob) };
                        out << Signal::SIGSRC_NOB << L' ' << idNobCompact << endl;
                    }
                    else
                        assert(false);
                }
            }
        );
    };
};
