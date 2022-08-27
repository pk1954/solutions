// WrapNrOfNobs.ixx
//
// ModelIO

module;

#include <iostream>
#include "IoConnector.h"
#include "NNetModelWriterInterface.h"
#include "UPNobList.h"

export module WrapNrOfNobs;

import NobIdList;
import NNetWrapperBase;
import Script;
import NNetModelIO;

using std::wostream;
using std::endl;

export class WrapNrOfNobs : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script& script) const final
    {
        script.ScrReadSpecial(L'=');
        long        lNrOfNobs{ script.ScrReadLong() };
        UPNobList& list{ m_modelIO.GetImportNMWI().GetUPNobs() };
        list.IncreaseSize(lNrOfNobs);
    }

    void Write(wostream& out) const final
    {
        out << L"NrOfNobs = " << m_modelIO.NrOfCompactIds() << endl;
    };
};
