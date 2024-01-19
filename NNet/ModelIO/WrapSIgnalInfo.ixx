// WrapSignalInfo.ixx
//
// ModelIO

module;

#include <cassert>
#include <string>
#include <iostream>

export module WrapSignalInfo;

import Wrapper;
import Script;
import Symtab;
import ErrHndl;
import Types;
import IoUtil;
import IoConstants;
import Signals;
import NNetWrapperHelpers;
import NNetModel;
import NNetModelIO;

using std::make_unique;
using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::endl;

export class WrapSignalInfo : public Wrapper
{
public:
    using Wrapper::Wrapper;

    explicit WrapSignalInfo(wstring const& wstrName)
      : Wrapper(wstrName)
    {
        SymbolTable::ScrDefConst(L"circle", static_cast<unsigned long>(Signal::SIGSRC_CIRCLE));
        SymbolTable::ScrDefConst(L"nob",    static_cast<unsigned long>(Signal::SIGSRC_NOB));
    };

    inline static wstring const SOURCE { L"source" };

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi     { NNetModelIO::GetImportNMWI() };
        SignalId             const signalId { ScrReadSignalId(script) };
        script.ScrReadString(SOURCE);
        unsigned long const ulSigSrc { script.ScrReadUlong() };
        if (ulSigSrc == Signal::SIGSRC_CIRCLE)
        {
            MicroMeterCircle umCircle = ScrReadMicroMeterCircle(script);
            SignalFactory::MakeSensorSignal(umCircle, signalId.GetTrackNr(), nmwi);
        }
        else if (ulSigSrc == Signal::SIGSRC_NOB)
        {
            NobId nobId { ScrReadNobId(script) };
            if (IsDefined(nobId))
            {
 		        unique_ptr<MicroSensor> upMicroSensor { make_unique<MicroSensor>(nmwi.GetConstNob(nobId)) };
                unique_ptr<NNetSignal>  upSignal      { make_unique<NNetSignal>(upMicroSensor.get()) };
		        nmwi.AddMicroSensor(move(upMicroSensor));
                nmwi.GetMonitorData().AddSignal(signalId.GetTrackNr(), move(upSignal));
            }
        }
        else
        {
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Signal source type must be 101"));
        }
    }

    void Write(wostream& out) const final
    {
        MonitorData const& monitorData{ NNetModelIO::GetExportNMRI().GetMonitorDataC() };
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
                        NobId       const  idNobCompact { NNetModelIO::GetCompactIdVal(idNob) };
                        out << Signal::SIGSRC_NOB << SPACE << idNobCompact << endl;
                    }
                    else
                        assert(false);
                }
            }
        );
    };
};
