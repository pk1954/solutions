// WrapTriggerSound.h 
//
// ModelIO

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"
#include "NNetWrapperHelpers.h"
#include "NNetWrapperBase.h"

class WrapTriggerSound : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        NobId const id      { ScrReadNobId(script) };
        Neuron    * pNeuron { m_modelIO.GetImportNMWI().GetNobPtr<Neuron *>(id) };
        Hertz const freq    { script.ScrReadUlong() };
        script.ScrReadString(L"Hertz");
        MilliSecs const msec { script.ScrReadUlong() };
        script.ScrReadString(L"msec");
    }

    void Write(wostream & out) const final 
    {
        m_modelIO.GetExportNMRI().Apply2AllC<Neuron>
        (
            [this, &out](Neuron const & neuron) 
            { 
                if (neuron.HasTriggerSound())
                {
                    SoundDescr sound { neuron.GetTriggerSound() };
                    WriteCmdName(out);
                    out << m_modelIO.GetCompactIdVal(neuron.GetId()) << L" "
                        << sound.m_frequency << L" Hertz "
                        << sound.m_duration  << L" msec "
                        << endl; 
                }
            } 
        );
    };
};
