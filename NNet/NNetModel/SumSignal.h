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
        Observable                     & observable,
        MicroMeterCircle         const & circle
    ) :
        SignalInterface( modelReaderInterface, param, observable )
    {
        m_circle = circle;
        Reset();
    }

    virtual ~SumSignal() {}

    virtual void Animate( bool const ) const;

    virtual void Draw( DrawContext const & ) const;

    virtual void WriteSignalData( wostream & ) const;

    virtual void Move( MicroMeterPoint const & umDelta )
    {
        m_circle += umDelta;
    }

    virtual void Size( float const factor )
    {
        m_circle *= factor;
    }

    virtual bool Includes( MicroMeterPoint const pos ) const 
    { 
        return m_circle.Includes( pos ); 
    }

    virtual bool MarkLineAboveThreshold( ) const { return false; }

    virtual MicroMeterPoint const GetCenter( ) const
    {
        return m_circle.GetPosition();
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
    MicroMeterCircle m_circle { MicroMeterCircle::NULL_VAL() };

    virtual float GetSignalValue( ) const;
};
