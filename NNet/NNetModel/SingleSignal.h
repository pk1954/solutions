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
        AnimationInterface             & animationInterface,
        ShapeId                  const   id
    ) :
      SignalInterface( modelReaderInterface, param, observable )
    {
        m_pAnimationInterface = & animationInterface;
        m_SignalShapeId = id;
        Reset();
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

    virtual bool MarkLineAboveThreshold( ) const { return true; }

    void WriteSignalData( wostream & out ) const
    {
        out << L"SingleSignal " << m_SignalShapeId;
    }

    ShapeId const GetSignalSource( ) const { return m_SignalShapeId; }

    virtual MicroMeterPoint const GetCenter( ) const
    {
        return m_pMRI->GetShapePos( m_SignalShapeId );
    }

    void CheckSignal( ) 
    {
#ifdef _DEBUG
        SignalInterface::CheckSignal();
        if ( ! m_pMRI->IsValidShapeId( m_SignalShapeId ) )
        {
            int x = 42;
        }
#endif
    }

private:
    ShapeId              m_SignalShapeId       { NO_SHAPE };
    AnimationInterface * m_pAnimationInterface { nullptr };

    virtual float GetSignalValue( ) const
    {
        Neuron const * pNeuron { m_pMRI->GetConstShapePtr<Neuron const *>( m_SignalShapeId ) };
        mV     const   voltage { pNeuron ? pNeuron->GetVoltage() : 0.0_mV };
        return voltage.GetValue();
    }

};
