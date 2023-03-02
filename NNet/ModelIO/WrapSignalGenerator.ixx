// WrapSignalGenerator.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapSignalGenerator;

import NNetWrapperBase;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

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
        out << L"\"" << pSigGen->GetName() << "\" ";
        out << data.GetFrequency() << data.GetAmplitude() << L' ' << data.GetPeakTime();
        out << endl;
    }
};
