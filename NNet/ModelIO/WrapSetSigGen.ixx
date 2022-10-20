// WrapSetSigGen.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapSetSigGen;

import NNetWrapperBase;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

using std::wstring;
using std::wostream;
using std::wcout;
using std::endl;

export class WrapSetSigGen : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi         { m_modelIO.GetImportNMWI() };
        NobId                const idFromScript { ScrReadNobId(script) };
        wstring              const nameSigGen   { script.ScrReadString() };
        SignalGenerator          * pSigGen      { nmwi.GetSigGen(nameSigGen) };
        bool                       bRes         { true };
        if (!nmwi.IsInputLine(idFromScript))
        {
            wcout << L"# +++ unexpected NobType.";
            bRes = false;
        }
        if (!pSigGen)
        {
            wcout << L"# +++ unknown signal generator.";
            bRes = false;
        }

        if (bRes)
            nmwi.GetNobPtr<InputLine*>(idFromScript)->SetSigGen(pSigGen);
        else
            wcout << L" Ignoring command" << endl;
    }

    void Write(wostream& out) const final
    {
        NNetModelReaderInterface const& nmri{ m_modelIO.GetExportNMRI() };
        nmri.Apply2AllC<InputLine>
        (
            [this, nmri, &out](InputLine const& inputLine)
            {
                SignalGenerator const* pSigGen{ inputLine.GetSigGenC() };
                if (pSigGen != StdSigGen::Get())
                {
                    WriteCmdName(out);
                    out << inputLine.GetId() << " "
                        << L"\"" << pSigGen->GetName() << "\" "
                        << endl;
                }
            }
        );
    };
};
