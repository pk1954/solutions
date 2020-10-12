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

    float const GetDataPoint( fMicroSecs const ) const;

    virtual void Notify( bool const );

    void CheckSignal( );

private:
    Param                    const * m_pParams               { nullptr };
    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    Observable                     * m_pObservable           { nullptr };

    fMicroSecs     m_timeStart     { 0._MicroSecs };
    vector <float> m_data          { };
    ShapeId        m_SignalShapeId { NO_SHAPE };
};
