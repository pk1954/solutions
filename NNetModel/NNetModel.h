// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "util.h"
#include "MoreTypes.h"
#include "PixelCoordsFp.h"
#include "Segment.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( );

	// readOnly functions

	BYTES        const GetCoreSize()            const { return BYTES(sizeof(NNetModel)); };
	microseconds const GetSimulationTime( )     const { return m_timeStamp; }
	ShapeId      const GetHighlightedShapeId( ) const { return m_shapeHighlighted; }

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, std::function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;
	
	Shape          * GetShape        ( ShapeId const );
	Shape    const * GetConstShape   ( ShapeId const ) const;
	Pipeline       * GetPipeline     ( ShapeId const ); 
	Pipeline const * GetConstPipeline( ShapeId const ) const;
	BaseKnot       * GetBaseKnot     ( ShapeId const ); 
	BaseKnot const * GetConstBaseKnot( ShapeId const ) const;
	Knot           * GetKnot         ( ShapeId const ); 
	Knot     const * GetConstKnot    ( ShapeId const ) const;

	void AddIncomming( NNetModel const &, ShapeId const, ShapeId const );
	void AddOutgoing ( NNetModel const &, ShapeId const, ShapeId const );
		
	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = microseconds( 0 ); }

	void CreateNewBranch      ( ShapeId const );
	void CreateNewNeuron      ( MicroMeterPoint const & );
	void CreateNewInputNeuron ( MicroMeterPoint const & );
	void CreateNewOutputNeuron( MicroMeterPoint const & );
	void SplitPipeline        ( ShapeId const, MicroMeterPoint const & );
	void Connect              ( NNetModel const & );

	void HighlightShape     ( ShapeId const );
	void SuperHighlightShape( ShapeId const );

	void Apply2AllShapes   ( std::function<void(Shape &)> const & ) const;
	void Apply2AllNeurons  ( std::function<void(Shape &)> const & ) const;
	void Apply2AllPipelines( std::function<void(Shape &)> const & ) const;

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	float        const GetDampingFactor( )      const { return m_dampingFactor; }  
	mV           const GetThresholdPotential( ) const { return m_thresholdPotential; }
	mV           const GetPeakVoltage( )        const { return m_peakVoltage; }    
	microseconds const GetPulseWidth( )         const { return m_pulseWidth; }     
	microseconds const GetRefractoryPeriod( )   const { return m_refractoryPeriod; } 
	meterPerSec  const GetImpulseSpeed( )       const { return m_impulseSpeed; } 

	void SetDampingFactor     ( float        const x ) { m_dampingFactor      = x; }  
	void SetThresholdPotential( mV           const x ) { m_thresholdPotential = x; }
	void SetPeakVoltage       ( mV           const x ) { m_peakVoltage        = x; }    
	void SetPulseWidth        ( microseconds const x ) { m_pulseWidth         = x; }     
	void SetRefractoryPeriod  ( microseconds const x ) { m_refractoryPeriod   = x; } 
	void SetImpulseSpeed      ( meterPerSec  const );

	bool IsBaseKnotType( ShapeId const id ) const
	{
		return ::IsBaseKnotType( GetConstShape( id )->GetShapeType() );
	}

private:
	// initial shapes 
	ShapeId m_idInputNeuron;
	ShapeId m_idOutputNeuron;
	ShapeId m_idPipeline;
			  
	// modal data
	vector<Shape *> m_Shapes;
	microseconds    m_timeStamp;

    // used by editor
	ShapeId m_shapeHighlighted;
	ShapeId m_shapeSuperHighlighted;

	// global parameters
	float        m_dampingFactor;     // signal loss per um  
    mV           m_thresholdPotential;
    mV           m_peakVoltage;   
    microseconds m_pulseWidth;   
    microseconds m_refractoryPeriod;
	meterPerSec  m_impulseSpeed;

	// local functions
	ShapeId const addShape( Shape * );
	void          deleteShape( ShapeId const );
	void          checkConsistency( Shape * ) const;
};
