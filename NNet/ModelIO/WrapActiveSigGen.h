// WrapActiveSigGen.h 
//
// ModelIO

#pragma once

#include <iostream>
#include "NNetModelReaderInterface.h"
#include "SigGenId.h"
#include "Script.h"

import NNetWrapperBase;

using std::wostream;
using std::endl;

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
