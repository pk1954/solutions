// WrapActiveSigGen.ixx
//
// ModelIO

export module WrapActiveSigGen;

import std;
import Commands;
import RunTime;
import Signals;
import NNetModelIO;
import NNetModel;

using std::wostream;
using std::endl;

export class WrapActiveSigGen : public Wrapper
{
public:
    using Wrapper::Wrapper;

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
