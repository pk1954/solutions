// SumSignal.h : 
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
#include "SignalInterface.h"
#include "Neuron.h"

using std::vector;

class SumSignal : public SignalInterface  
{
public:

    SumSignal
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Param                    const & param,
        Observable                     & observable
    ) :
        SignalInterface( modelReaderInterface, param, observable )
    {}

    virtual ~SumSignal() {}

    virtual void Animate( bool const ) const;

    virtual void Draw( DrawContext const &  ) const;

    virtual void WriteSignalData( wostream & ) const;

    void SetSignalSource( MicroMeterCircle const circle )
    {
        m_circle = circle;
        Reset();
    }

    bool IsInCircle( MicroMeterPoint const pos ) const 
    { 
        return m_circle.Includes( pos ); 
    }

    bool IsOnBorder( MicroMeterPoint const pos ) const
    {
        return IsInCircle( pos )  && ! (m_circle * BORDER).Includes( pos );
    }

    void Set2Null() { m_circle.Set2Null(); }

    void Check( ) const {};

    void Dump( ) const {};

    void CheckSignal( ) 
    {
#ifdef _DEBUG
        SignalInterface::CheckSignal();
        if ( m_circle.IsNull() )
        {
            int x = 42;
        }
#endif
    }

private:
    inline static const float BORDER { 0.8f };

    MicroMeterCircle m_circle { MicroMeterCircle::NULL_VAL() };

    virtual float GetSignalValue( ) const;
};
