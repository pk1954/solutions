// WrapperBase.h 
//
// ModelIO

#pragma once

#include "ScriptFunctor.h"
#include "NNetModelImporter.h"

class NNetModelWriterInterface;
class MonitorData;
class UPNobList;

class WrapperBase : public ScriptFunctor
{
public:
    explicit WrapperBase(NNetModelImporter & modelImporter) :
        m_modelImporter(modelImporter)
    { };

protected:
    NNetModelWriterInterface & GetWriterInterface() const { return m_modelImporter.GetWriterInterface(); }
    MonitorData              & GetMonitorData()     const { return m_modelImporter.GetWriterInterface().GetMonitorData(); }
    UPNobList                & GetUPNobsRef()       const { return m_modelImporter.GetWriterInterface().GetUPNobs(); }

    template <Nob_t T> T& GetNobRef(NobId const id) const
    {
        return * GetWriterInterface().GetNobPtr<T*>(id);
    }
private:
    NNetModelImporter & m_modelImporter;
};
