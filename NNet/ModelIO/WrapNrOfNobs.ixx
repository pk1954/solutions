// WrapNrOfNobs.ixx
//
// ModelIO

module;

#include <cassert>
#include <iostream>

export module WrapNrOfNobs;

import WrapBase;
import Script;
import NNetModelIO;
import NNetModel;
import IoUtil;
import IoConstants;

using std::wostream;
using std::endl;

export class WrapNrOfNobs : public WrapBase
{
public:
    using WrapBase::WrapBase;

    void operator() (Script& script) const final
    {
        script.ScrReadSpecial(EQUALS);
        long       lNrOfNobs { script.ScrReadLong() };
        UPNobList& list      { NNetModelIO::GetImportNMWI().GetUPNobs() };
        assert(list.IsEmpty());
        list.IncreaseSize(lNrOfNobs);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << EQUALS << SPACE << NNetModelIO::NrOfCompactIds() << endl;
    }
};
