// WrapActiveSigGen.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapActiveSigGen;

import WrapBase;
import Script;
import NNetModelIO;
import NNetModel;

using std::wostream;
using std::endl;

export class WrapActiveSigGen : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi { NNetModelIO::GetImportNMWI() };
        SigGenId            const  id   { SigGenId(script.ScrReadInt()) };
        if (nmwi.IsValid(id))
            nmwi.SetSigGenActive(id);
    }

    void Write(wostream & out) const final
    {
        WriteCmdName(out);
        out << NNetModelIO::GetExportNMRI().GetSigGenIdSelected() << endl;
    };
};
