// WrapSignalData.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapSignalData;

import NNetWrapperBase;
import IoUtil;
import SaveCast;
import Types;
import Script;
import NNetModel;
import NNetWrapperHelpers;

using std::wostream;
using std::endl;

export class WrapSignalData : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        SignalId   const signalId { ScrReadSignalId(script) };
        Signal         * pSignal  { m_modelIO.GetImportNMWI().GetMonitorData().GetSignalPtr(signalId) };
        { script.ScrReadString(L"StartTime"); };
        fMicroSecs const umStartTime{ Cast2Float(script.ScrReadFloat()) };
        if (pSignal)
            pSignal->SetStartTime(umStartTime);
        { script.ScrReadSpecial(LIST_OPEN_BRACKET); }
        int const iNrOfElements{ script.ScrReadInt() };
        if (pSignal)
            pSignal->Reserve(iNrOfElements);
        script.ScrReadSpecial(NR_SEPARATOR);
        for (int iElem = 0; iElem < iNrOfElements; ++iElem)
        {
            mV value{ Cast2Float(script.ScrReadFloat()) };
            if (pSignal)
                pSignal->Add(value);
        }
        script.ScrReadSpecial(LIST_CLOSE_BRACKET);
    }

    void Write(wostream& out) const final
    {
        MonitorData const& monitorData{ m_modelIO.GetExportNMRI().GetMonitorDataC() };
        monitorData.Apply2AllSignalIdsC
        (
            [this, &out, &monitorData](SignalId const idSignal)
            {
                if (Signal const * const pSignal{ monitorData.GetConstSignalPtr(idSignal) })
                {
                    WriteCmdName(out);
                    out << idSignal;
                    out << L" StartTime " << pSignal->GetStartTime().GetValue() << endl;
                    pSignal->WriteSignalData(out);
                }
            }
        );
    };
};
