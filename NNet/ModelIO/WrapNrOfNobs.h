// WrapNrOfNobs.h 
//
// ModelIO

#pragma once

#include "IoConnector.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"

import NobIdList;
import NNetWrapperBase;

class WrapNrOfNobs : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        script.ScrReadSpecial(L'=');
        long        lNrOfNobs { script.ScrReadLong() };
        UPNobList & list      { m_modelIO.GetImportNMWI().GetUPNobs() };
        list.IncreaseSize(lNrOfNobs);
    }

    void Write(wostream & out) const final 
    {
        out << L"NrOfNobs = " << m_modelIO.NrOfCompactIds() << endl;
    };
};
