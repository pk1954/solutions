// NNetModelWrapBase.h 
//
// ModelIO

#pragma once

#include "NNetModelWriterInterface.h"
#include "NNetModelIO.h"
#include "WrapBase.h"

class MonitorData;
class UPNobList;

class NNetModelWrapBase : public WrapBase
{
public:
    explicit NNetModelWrapBase
    (
        NNetModelIO   & modelIO,
        wstring const & wstrName
    )
      : WrapBase(wstrName),
        m_modelIO(modelIO)
    { };

protected:
    NNetModelWriterInterface & GetWriterInterface() const { return m_modelIO.GetWriterInterface(); }
    MonitorData              & GetMonitorData()     const { return m_modelIO.GetWriterInterface().GetMonitorData(); }
    UPNobList                & GetUPNobsRef()       const { return m_modelIO.GetWriterInterface().GetUPNobs(); }
    UPSensorList             & GetUPSensorsRef()    const { return m_modelIO.GetWriterInterface().GetSensorList(); }

    template <Nob_t T> T& GetNobRef(NobId const id) const
    {
        return * GetWriterInterface().GetNobPtr<T*>(id);
    }
private:
    NNetModelIO & m_modelIO;
};
