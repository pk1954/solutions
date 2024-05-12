// WrapSignalData.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapSignalData;

import Wrapper;
import IoUtil;
import IoConstants;
import SaveCast;
import Types;
import Script;
import Signals;
import NNetModel;
import NNetModelIO;
import NNetWrapperHelpers;

using std::wostream;
using std::endl;

export class WrapSignalData : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        SignalId   const signalId { ScrReadSignalId(script) };
        Signal         * pSignal  { NNetModelIO::GetImportNMWI().GetMonitorData().GetSignalPtr(signalId) };
        { script.ScrReadString(L"StartTime"); };
        fMicroSecs const umStartTime{ Cast2Float(script.ScrReadFloat()) };
        if (pSignal)
            pSignal->SetStartTime(umStartTime);
        { script.ScrReadSpecial(CURLY_OPEN_BRACKET); }
        int const iNrOfElements{ script.ScrReadInt() };
        if (pSignal)
            pSignal->Reserve(iNrOfElements);
        script.ScrReadSpecial(NR_SEPARATOR);
        for (int iElem = 0; iElem < iNrOfElements; ++iElem)
        {
            mV value{ Cast2Float(script.ScrReadFloat()) };
            if (pSignal)
                pSignal->Add2Signal(value);
        }
        script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
    }

    void Write(wostream& out) const final
    {
        NNetModelReaderInterface const &nmri { NNetModelIO::GetExportNMRI() };
        if (nmri.ModelLocked())
            return;

        MonitorData const& monitorData { nmri.GetMonitorDataC() };
        monitorData.Apply2AllSignalIdsC
        (
            [this, &out, &monitorData](SignalId const idSignal)
            {
                if (Signal const * const pSignal{ monitorData.GetConstSignalPtr(idSignal) })
                {
                    WriteCmdName(out);
                    out << idSignal;
                    out << L" StartTime" << pSignal->GetStartTime() << endl;
                    pSignal->WriteSignalData(out);
                }
            }
        );
    };
};
