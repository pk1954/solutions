// WrapSignalData.h 
//
// ModelIO

#pragma once

#include "NNetWrapperBase.h"

class WrapSignalData : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        SignalId   const signalId    { ScrReadSignalId(script) };
        Signal         * pSignal     { m_modelIO.GetImportNMWI().GetMonitorData().GetSignalPtr(signalId) };
        script.ScrReadString(L"StartTime");
        fMicroSecs const umStartTime { Cast2Float(script.ScrReadFloat()) };
        pSignal->SetStartTime(umStartTime);
        script.ScrReadSpecial(LIST_OPEN_BRACKET);
        int const iNrOfElements { script.ScrReadInt() };
        script.ScrReadSpecial(NR_SEPARATOR);
        for (int iElem = 0; iElem < iNrOfElements; ++iElem)
        {
            mV value { Cast2Float(script.ScrReadFloat()) };
            pSignal->Add(value);
        }
        script.ScrReadSpecial(LIST_CLOSE_BRACKET);
    }

    void Write(wostream & out) const final 
    {
        MonitorData const & monitorData { m_modelIO.GetExportNMWI().GetConstMonitorData() };
        monitorData.Apply2AllSignalIdsC
        (
            [this, &out, &monitorData](SignalId const idSignal)
            {
                if (Signal const * const pSignal { monitorData.GetConstSignalPtr(idSignal) })
                {
                    WriteCmdName(out);
                    out << idSignal; 
                    pSignal->WriteSignalData(out);
                }
            }
        );
    };
};
