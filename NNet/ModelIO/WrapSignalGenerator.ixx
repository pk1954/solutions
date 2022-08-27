// WrapSignalGenerator.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>
#include "NNetModelWriterInterface.h"
#include "NNetWrapperHelpers.h"
#include "SignalGenerator.h"

export module WrapSignalGenerator;

import NNetWrapperBase;
import SigGenId;
import SigGenStaticData;
import NNetModelIO;

using std::wstring;
using std::wostream;
using std::endl;

export class WrapSignalGenerator : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi       { m_modelIO.GetImportNMWI() };
        wstring              const name       { script.ScrReadString() };
        SigGenStaticData     const sigGenData { ScrReadSigGenStaticData(script) };
        SigGenId             const sigGenId   { nmwi.FindSigGen(name) };
        if (nmwi.IsValid(sigGenId))
        {
            SignalGenerator * pSigGen{ nmwi.GetSigGen(sigGenId) };
            pSigGen->SetStaticData(sigGenData);
        }
        else
        {
            UPSigGen upSigGen { nmwi.NewSigGen(name) };
            upSigGen->SetStaticData(sigGenData);
            m_modelIO.GetImportNMWI().PushSigGen(move(upSigGen));
        }
    }

    void Write(wostream & out) const final
    {
        m_modelIO.GetExportNMRI().GetSigGenList().Apply2AllC
        (
            [this, &out](auto const& upSigGen)
            {
                WriteCmdName(out);
                out << L"\"" << upSigGen->GetName() << "\" "
                    << upSigGen->GetStaticData()
                    << endl;
            }
        );
    };
};
