// SingleSignal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "Observable.h"
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "AnimationInterface.h"
#include "NNetModelReaderInterface.h"
#include "SignalInterface.h"
#include "Neuron.h"

using std::vector;

class SingleSignal : public SignalInterface  
{
public:

    SingleSignal
    ( 
        NNetModelReaderInterface const & modelReaderInterface,
        Param                    const & param,
        Observable                     & observable,
        AnimationInterface             & animationInterface
    ) :
      SignalInterface( modelReaderInterface, param, observable )
    {
        m_pAnimationInterface = & animationInterface;
    }

    virtual ~SingleSignal() {}

    virtual void Animate( bool const bOn ) const
    {
        if ( bOn )
            m_pAnimationInterface->Start( m_SignalShapeId );
        else
            m_pAnimationInterface->Stop( );
    }

    virtual void Draw( DrawContext const & ) const {};

    void WriteSignalData( wostream & out ) const
    {
        out << L" \"shape\" " << m_SignalShapeId;
    }

    ShapeId const GetSignalSource( ) const { return m_SignalShapeId; }

    void SetSignalSource( ShapeId const id )
    {
        m_SignalShapeId = id;
        Reset();
    }

    void CheckSignal( ) 
    {
#ifdef _DEBUG
        SignalInterface::CheckSignal();
        if ( ! m_pModelReaderInterface->IsValidShapeId( m_SignalShapeId ) )
        {
            int x = 42;
        }
#endif
    }

private:
    ShapeId              m_SignalShapeId    { NO_SHAPE };
    AnimationInterface * m_pAnimationInterface { nullptr };

    virtual float GetSignalValue( ) const
    {
        Neuron const * pNeuron { m_pModelReaderInterface->GetConstShapePtr<Neuron const *>( m_SignalShapeId ) };
        mV     const   voltage { pNeuron ? pNeuron->GetVoltage() : 0.0_mV };
        return voltage.GetValue();
    }

};
