// WrapSignalGenerator.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapSignalGenerator;

import WrapBase;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;
import IoUtil;

using std::wstring;
using std::wostream;
using std::endl;

export class WrapSignalGenerator : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi       { NNetModelIO::GetImportNMWI() };
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
            NNetModelIO::GetImportNMWI().PushSigGen(move(upSigGen));
        }
    }

    void Write(wostream & out) const final
    {
        NNetModelIO::GetExportNMRI().GetSigGenList().Apply2AllC
        (
            [this, &out](auto const& pSigGen)
            {
                WriteCmdName(out);
                writeSigGen(out, pSigGen);
            }
        );
    };

private:
    void writeSigGen
    (
        wostream& out,
        SignalGenerator const* pSigGen
    ) const
    {
        SigGenStaticData const& data { pSigGen->GetStaticData() };
        pSigGen->WriteName(out);
        out << data.GetFrequency();
        out << data.GetAmplitude();
        out << data.GetPeakTime().GetValue();
        out << endl;
    }
};
