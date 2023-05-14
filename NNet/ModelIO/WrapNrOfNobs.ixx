// WrapNrOfNobs.ixx
//
// ModelIO

module;

#include <cassert>
#include <iostream>

export module WrapNrOfNobs;

import NNetWrapperBase;
import Script;
import NNetModelIO;
import NNetModel;
import IoUtil;

using std::wostream;
using std::endl;

export class WrapNrOfNobs : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        script.ScrReadSpecial(EQUALS);
        long       lNrOfNobs { script.ScrReadLong() };
        UPNobList& list      { m_modelIO.GetImportNMWI().GetUPNobs() };
        assert(list.IsEmpty());
        list.IncreaseSize(lNrOfNobs);
    }

    void Write(wostream& out) const final
    {
        WriteCmdName(out);
        out << EQUALS << SPACE << m_modelIO.NrOfCompactIds() << endl;
    }
};
