// ProbeHead.h : 
//
// NNetModel

#pragma once

#include "ObserverInterface.h"
#include "NNetModelReaderInterface.h"
#include "Neuron.h"
#include "Signal.h"

class ProbeHead : public ObserverInterface
{
public:

    void Initialize( NNetModelReaderInterface const & modelReaderInterface )
    {
        m_pModelReaderInterface = & modelReaderInterface;
    }

    void AttachSignal( Signal * pSignal )
    {
        m_pSignal = pSignal;
    }

    void SetSignalSource( Neuron const & neuron )
    {
        m_pSignalSource = & neuron;
        m_pSignal->Reset( m_pModelReaderInterface->GetSimulationTime( ) );
    }

    virtual void Notify( bool const bImmediate )
    {
        if ( m_pSignalSource )
        {
            m_pSignal->Add( m_pSignalSource->GetVoltage().GetValue() );
        }
    }

private:

    Neuron                   const * m_pSignalSource         { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Signal                         * m_pSignal               { nullptr };
};
