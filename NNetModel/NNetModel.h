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

	void AddIncomming( ShapeId const, ShapeId const );
	void AddOutgoing ( ShapeId const, ShapeId const );
		
	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = microseconds( 0 ); }

	ShapeId const AddInputNeuron ( MicroMeterPoint const & );
	ShapeId const AddOutputNeuron( MicroMeterPoint const & );
	ShapeId const AddNeuron      ( MicroMeterPoint const & );
	ShapeId const AddKnot        ( MicroMeterPoint const & );
	ShapeId const AddPipeline    ( meterPerSec     const );

	void CreateNewBranch( ShapeId const );
	void CreateNewNeuron( MicroMeterPoint const & );
	void CreateNewInputNeuron( MicroMeterPoint const & );
	void CreateNewOutputNeuron( MicroMeterPoint const & );
	void SplitPipeline( ShapeId const, MicroMeterPoint const & );
	void Connect( );

	void HighlightShape     ( ShapeId const );
	void SuperHighlightShape( ShapeId const );

	void Apply2AllShapes   ( std::function<void(Shape &)> const & ) const;
	void Apply2AllNeurons  ( std::function<void(Shape &)> const & ) const;
	void Apply2AllPipelines( std::function<void(Shape &)> const & ) const;

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	// static functions

	static float        const GetDampingFactor( )      { return m_dampingFactor; }  
	static mV           const GetThresholdPotential( ) { return m_thresholdPotential; }
	static mV           const GetPeakVoltage( )        { return m_peakVoltage; }    
	static microseconds const GetPulseWidth( )         { return m_pulseWidth; }     
	static microseconds const GetRefractoryPeriod( )   { return m_refractoryPeriod; } 
	    
	static void SetDampingFactor     ( float        const x ) { m_dampingFactor      = x; }  
	static void SetThresholdPotential( mV           const x ) { m_thresholdPotential = x; }
	static void SetPeakVoltage       ( mV           const x ) { m_peakVoltage        = x; }    
	static void SetPulseWidth        ( microseconds const x ) { m_pulseWidth         = x; }     
	static void SetRefractoryPeriod  ( microseconds const x ) { m_refractoryPeriod   = x; } 
																									   
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
	static float        m_dampingFactor;     // signal loss per um  
    static mV           m_thresholdPotential;
    static mV           m_peakVoltage;   
    static microseconds m_pulseWidth;   
    static microseconds m_refractoryPeriod;

	// local functions
	ShapeId const addShape( Shape * );
	void          deleteShape( ShapeId const );
	void          checkConsistency( Shape * ) const;
};
