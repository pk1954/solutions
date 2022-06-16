// WrapActiveSigGen.h 
//
// ModelIO

#pragma once

#include "NNetWrapperBase.h"

class WrapActiveSigGen : public NNetWrapperBase 
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        NNetModelWriterInterface & nmwi { m_modelIO.GetImportNMWI() };
        SigGenId           const   id   { SigGenId(script.ScrReadUint()) };
        if (nmwi.IsValid(id))
            nmwi.SetSigGenActive(id);
    }

    void Write(wostream & out) const final 
    {
        WriteCmdName(out);
        out << m_modelIO.GetExportNMRI().GetSigGenIdSelected() << endl;
    };
};
