// NNetWrapperBase.h 
//
// ModelIO

#pragma once

#include "NNetModelWriterInterface.h"

import WrapBase;

class MonitorData;
class UPNobList;
class NNetModelIO;

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
