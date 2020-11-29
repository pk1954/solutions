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
    );

    virtual ~Signal();

    void SetSignalSource( ShapeId const );

    ShapeId    const GetSignalSource( ) const { return m_SignalShapeId;  }
    fMicroSecs const GetStartTime   ( ) const { return m_timeStart; }

    float      const GetDataPoint   ( fMicroSecs const ) const;
    fMicroSecs const FindNextMaximum( fMicroSecs const ) const;

    virtual void Notify( bool const );

    void CheckSignal( );

private:
    Param                    const * m_pParams               { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Observable                     * m_pObservable           { nullptr };

    int time2index( fMicroSecs const usParam ) const
    {
        fMicroSecs const timeTilStart { usParam - m_timeStart };
        float      const fNrOfPoints  { timeTilStart / m_pParams->GetTimeResolution( ) };
        int              index        { static_cast<int>( roundf( fNrOfPoints ) ) };
        if ( index >= m_data.size() )
            index = Cast2UnsignedInt( m_data.size() - 1 );
        return index;
    }

    fMicroSecs const index2time( int const index ) const
    {
        float      const fNrOfPoints  { static_cast<float>( index ) };
        fMicroSecs const timeTilStart { m_pParams->GetTimeResolution( ) * fNrOfPoints };
        fMicroSecs const usResult     { timeTilStart + m_timeStart };
        return usResult;
    }

    fMicroSecs     m_timeStart     { 0._MicroSecs };
    vector <float> m_data          { };
    ShapeId        m_SignalShapeId { NO_SHAPE };
};
