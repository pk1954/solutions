// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "Observable.h"
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "Neuron.h"

using std::vector;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Param                    const & param,
        Observable                     & observable
    )
    {
        m_pModelReaderInterface = & modelReaderInterface;
        m_pParams = & param;
        m_pObservable = & observable;
        m_pObservable->RegisterObserver( this );
    }

    virtual ~Signal( )
    {
        m_pObservable->UnregisterObserver( this );
    }

    void SetSignalSource( ShapeId const id )
    {
        m_SignalShapeId = id;
        m_data.clear();
        m_timeStart = m_pModelReaderInterface->GetSimulationTime( );
    }

    ShapeId GetSignalSource( ) const
    {
        return m_SignalShapeId;
    }

    fMicroSecs const GetStartTime( ) const
    {
        return m_timeStart;
    }

    float const GetDataPoint( unsigned int uiIndex ) const
    {
        if ( m_data.empty() )
            return NAN;
        if ( uiIndex >= m_data.size() )
            uiIndex = CastToUnsignedInt( m_data.size() - 1 );
        return m_data[ uiIndex ];
    }

    unsigned int GetPointIndex( fMicroSecs const time ) const
    {
        fMicroSecs const timeTilStart { time - m_timeStart };
        float      const fNrOfPoints  { timeTilStart / m_pParams->GetTimeResolution( ) };
        return static_cast<unsigned int>( roundf( fNrOfPoints ) );
    }

    float const GetDataPoint( fMicroSecs const time ) const
    {
        return GetDataPoint( GetPointIndex( time ) );
    }

    virtual void Notify( bool const bImmediate )
    {
        Neuron const * pNeuron { m_pModelReaderInterface->GetConstShapePtr<Neuron const *>( m_SignalShapeId ) };
        mV     const   voltage { pNeuron ? pNeuron->GetVoltage() : 0.0_mV };
        m_data.push_back( voltage.GetValue() );
    }

private:
    fMicroSecs                       m_timeStart             { 0._MicroSecs };
    vector <float>                   m_data                  { };
    ShapeId                          m_SignalShapeId         { NO_SHAPE };
    Param                    const * m_pParams               { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Observable                     * m_pObservable           { nullptr };
};
