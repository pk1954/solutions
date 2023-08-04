// WrapSetSigGen.ixx
//
// ModelIO

module;

#include <string>
#include <iostream>

export module WrapSetSigGen;

import WrapBase;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;
import IoUtil;
import IoConstants;

using std::wstring;
using std::wostream;
using std::wcout;
using std::endl;

export class WrapSetSigGen : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        NNetModelWriterInterface & nmwi         { NNetModelIO::GetImportNMWI() };
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
        NNetModelReaderInterface const& nmri{ NNetModelIO::GetExportNMRI() };
        nmri.Apply2AllC<InputLine>
        (
            [this, nmri, &out](InputLine const& inputLine)
            {
                SignalGenerator const* pSigGen{ inputLine.GetSigGenC() };
                if (pSigGen != StdSigGen::Get())
                {
                    WriteCmdName(out);
                    out << NNetModelIO::GetCompactIdVal(inputLine.GetId()) << SPACE;
                    pSigGen->WriteName(out);
                    out << endl;
                }
            }
        );
    };
};
