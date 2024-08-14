// WrapNrOfNobs.ixx
//
// ModelIO

export module WrapNrOfNobs;

import std;
import Debug;
import Commands;
import RunTime;
import NNetModelIO;
import NNetModel;
import IoUtil;
import IoConstants;

using std::endl;

export class WrapNrOfNobs : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        script.ScrReadSpecial(EQUALS);
        long       lNrOfNobs { script.ScrReadLong() };
        UPNobList& list      { NNetModelIO::GetImportNMWI().GetUPNobs() };
        Assert(list.IsEmpty());
        list.IncreaseSize(lNrOfNobs);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << EQUALS << SPACE << NNetModelIO::NrOfCompactIds() << endl;
    }
};
