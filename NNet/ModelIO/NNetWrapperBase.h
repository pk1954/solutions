// NNetWrapperBase.h 
//
// ModelIO

#pragma once

#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "UPNobList.h"
#include "NNetModelIO.h"

import WrapBase;

class NNetWrapperBase : public WrapBase
{
public:
    explicit NNetWrapperBase
    (
        wstring const & wstrName,
        NNetModelIO   & modelIO
    )
      : WrapBase(wstrName),
        m_modelIO(modelIO)
    {};

    void Write(wostream & out) const override { /* for legacy commands */ };

protected:

    NNetModelIO & m_modelIO;
};
