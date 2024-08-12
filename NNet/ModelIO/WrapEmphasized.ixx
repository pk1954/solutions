// WrapEmphasized.ixx
//
// ModelIO

export module WrapEmphasized;

import std;
import Commands;
import NNetWrapperHelpers;
import RunTime;
import NNetModel;
import NNetModelIO;

using std::wostream;
using std::endl;

export class WrapEmphasized : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface& nmwi  { NNetModelIO::GetImportNMWI() };
        NobId               const idNob { ScrReadNobId(script) };
        Nob *               const pNob  { nmwi.GetNobPtr<Nob*>(idNob) };
        pNob->Emphasize(true);
    }

    void Write(wostream& out) const final
    {
        NNetModelIO::GetExportNMRI().Apply2AllC
        (
            [this, &out](Nob const& nob) 
            { 
                if (nob.IsEmphasized())
                {
                    WriteCmdName(out);
                    out << NNetModelIO::GetCompactIdVal(nob.GetId()) << endl;
                }
            }
        );
    };
};
