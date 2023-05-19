// WrapSignalInfo.ixx
//
// ModelIO

module;

#include <cassert>
#include <string>
#include <iostream>

export module WrapSignalInfo;

import NNetWrapperBase;
import Script;
import Symtab;
import ErrHndl;
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

    explicit WrapSignalInfo
    (
        wstring const& wstrName,
        NNetModelIO& modelIO
    )
      : NNetWrapperBase(wstrName, modelIO)
    {
        SymbolTable::ScrDefConst(L"circle", static_cast<unsigned long>(Signal::SIGSRC_CIRCLE));
        SymbolTable::ScrDefConst(L"nob",    static_cast<unsigned long>(Signal::SIGSRC_NOB));
    };

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
                if (NNetSignal const* const pSignal{ static_cast<NNetSignal const *>(monitorData.GetConstSignalPtr(idSignal)) })
                {
                    WriteCmdName(out);
                    out << idSignal << SPACE << SOURCE << SPACE;
                    NNetSignalSource const* pSigSrc { pSignal->GetSignalSource() };
                    NNetSignalSource::Type  type    { pSigSrc->SignalSourceType() };
                    if (type == NNetSignalSource::Type::macroSensor)
                    {
                        static_cast<Sensor const *>(pSigSrc)->WriteInfo(out);
                    }
                    else if (type == NNetSignalSource::Type::microSensor)
                    {
                        MicroSensor const& microSensor  { static_cast<MicroSensor const&>(*pSigSrc) };
                        NobId       const  idNob        { microSensor.GetNobId() };
                        NobId       const  idNobCompact { m_modelIO.GetCompactIdVal(idNob) };
                        out << Signal::SIGSRC_NOB << SPACE << idNobCompact << endl;
                    }
                    else
                        assert(false);
                }
            }
        );
    };
};
