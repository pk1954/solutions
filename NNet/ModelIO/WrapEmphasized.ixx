// WrapEmphasized.ixx
//
// ModelIO

module;

#include <iostream>

export module WrapEmphasized;

import NNetWrapperBase;
import NNetWrapperHelpers;
import Script;
import NNetModel;

using std::wostream;
using std::endl;

export class WrapEmphasized : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface& nmwi  { m_modelIO.GetImportNMWI() };
        NobId               const idNob { ScrReadNobId(script) };
        Nob *               const pNob  { nmwi.GetNobPtr<Nob*>(idNob) };
        pNob->Emphasize(true);
    }

    void Write(wostream& out) const final
    {
        m_modelIO.GetExportNMRI().Apply2AllC
        (
            [this, &out](Nob const& nob) 
            { 
                if (nob.IsEmphasized())
                {
                    WriteCmdName(out);
                    out << nob.GetId() << endl;
                }
            }
        );
    };
};
