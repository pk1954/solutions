// WrapSetScanArea.ixx
//
// ModelIO

export module WrapSetScanArea;

import std;
import IoUtil;
import NNetModelIO;
import Commands;

using std::endl;

export class WrapSetScanArea : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        NNetModelIO::GetImportNMWI().SetScanArea(ScrReadMicroMeterRect(script));
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << NNetModelIO::GetExportNMRI().GetScanArea() << endl;
    }
};
