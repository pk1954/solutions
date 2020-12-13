// SignalInterface.h : 
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

class SignalInterface : public ObserverInterface  // observes signal source 
{
public:

    SignalInterface
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Param                    const & param,
        Observable                     & observable
    );

    virtual ~SignalInterface();

    fMicroSecs const GetStartTime( ) const { return m_timeStart; }

    float      const GetDataPoint   ( fMicroSecs const ) const;
    fMicroSecs const FindNextMaximum( fMicroSecs const ) const;

    virtual void Notify( bool const );
    
    virtual void  WriteSignalData( wostream & ) const = 0;
    virtual float GetSignalValue()              const = 0;
    virtual void  Animate( bool const )         const = 0;
    virtual void  Draw( DrawContext const & )   const = 0;

    void CheckSignal( );

protected:

    NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
    fMicroSecs                       m_timeStart { 0._MicroSecs };
    vector <float>                   m_data      { };

    void Reset( )
    {
        m_data.clear();
        m_timeStart = m_pModelReaderInterface->GetSimulationTime( );
    }

private:
    Param const * m_pParams     { nullptr };
    Observable  * m_pObservable { nullptr };

    int        const time2index( fMicroSecs const ) const;
    fMicroSecs const index2time( int        const ) const;
};
