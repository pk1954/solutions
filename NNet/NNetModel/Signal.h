// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "Observable.h"
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "AnimationInterface.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"

using std::vector;

class Signal;

using SignalFunc = function<void(Signal const &)>;
using SignalCrit = function<bool(Signal const &)>;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal
    ( 
        NNetModelReaderInterface const &,
        Param                    const &,
        Observable                     &,
        AnimationInterface             &,
        MicroMeterCircle         const & 
    );

    virtual ~Signal();

    fMicroSecs const GetStartTime( ) const { return m_timeStart; }
    float      const GetDataPoint   ( fMicroSecs const ) const;
    fMicroSecs const FindNextMaximum( fMicroSecs const ) const;

    void Notify( bool const );
    void Animate( bool const ) const;
    void Draw( DrawContext const & ) const;
    float GetSignalValue( ) const;
    void  WriteSignalData( wostream & ) const;;

    bool Includes( MicroMeterPoint const pos ) const { return m_circle.Includes( pos ); }

    void Move( MicroMeterPoint const & umDelta ) { m_circle += umDelta; }
    void Size( float           const   factor  ) { m_circle *= factor; }

    MicroMeterPoint const GetCenter( ) const  { return m_circle.GetPosition(); }

    void Set2Null() { m_circle.Set2Null(); }

    void Check( ) const {};
    void Dump( ) const {};
    void CheckSignal();

private:

    NNetModelReaderInterface const * m_pMRI                { nullptr };
    Param                    const * m_pParams             { nullptr };
    Observable                     * m_pObservable         { nullptr };
    AnimationInterface             * m_pAnimationInterface { nullptr };
    MicroMeterCircle                 m_circle              { MicroMeterCircle::NULL_VAL() };
    fMicroSecs                       m_timeStart           { 0._MicroSecs };
    vector <float>                   m_data                { };

    void Reset( )
    {
        m_data.clear();
        m_timeStart = m_pMRI->GetSimulationTime( );
    }

    int        const time2index( fMicroSecs const ) const;
    fMicroSecs const index2time( int        const ) const;
};
