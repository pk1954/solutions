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

enum class tParameter
{
	pulseRate,
	pulseSpeed,
	pulseWidth,
	dampingFactor,
	threshold,
	peakVoltage,
	refractoryPeriod,
	tParameterLast = refractoryPeriod
};

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( );

	class ModelInconsistencyException : public std::exception {};
	static ModelInconsistencyException const ModelInconsistency;

	// readOnly functions

	BYTES     const GetCoreSize()            const { return BYTES(sizeof(NNetModel)); };
	MicroSecs const GetSimulationTime( )     const { return m_timeStamp; }
	ShapeId   const GetHighlightedShapeId( ) const { return m_shapeHighlighted; }

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, std::function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;
	
	Shape              * GetShape            ( ShapeId const );
	Shape        const * GetConstShape       ( ShapeId const ) const;
	Pipeline           * GetPipeline         ( ShapeId const ); 
	Pipeline     const * GetConstPipeline    ( ShapeId const ) const;
	BaseKnot           * GetBaseKnot         ( ShapeId const ); 
	BaseKnot     const * GetConstBaseKnot    ( ShapeId const ) const;
	Knot               * GetKnot             ( ShapeId const ); 
	Knot         const * GetConstKnot        ( ShapeId const ) const;
	InputNeuron        * GetInputNeuron      ( ShapeId const );
	InputNeuron  const * GetConstInputNeuron ( ShapeId const ) const;
	OutputNeuron       * GetOutputNeuron     ( ShapeId const );
	OutputNeuron const * GetConstOutputNeuron( ShapeId const ) const;

	wchar_t const * const GetParameterLiteral( tParameter const ) const;
	wchar_t const * const GetParameterName   ( tParameter const ) const;
	wchar_t const * const GetParameterUnit   ( tParameter const ) const;

	float const GetParameterValue( tParameter const, Shape const * const = nullptr ) const;

	bool IsPipelineType ( ShapeId const id ) const { return ::IsPipelineType ( GetConstShape( id )->GetShapeType() ); }
	bool IsBaseKnotType ( ShapeId const id ) const { return ::IsBaseKnotType ( GetConstShape( id )->GetShapeType() ); }
	bool IsStartKnotType( ShapeId const id ) const { return ::IsStartKnotType( GetConstShape( id )->GetShapeType() ); }
	bool IsEndKnotType  ( ShapeId const id ) const { return ::IsEndKnotType  ( GetConstShape( id )->GetShapeType() ); }

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	ShapeId NewInputNeuron ( MicroMeterPoint const & );
	ShapeId NewOutputNeuron( MicroMeterPoint const & );
	ShapeId NewNeuron      ( MicroMeterPoint const & );
	ShapeId NewKnot        ( MicroMeterPoint const & );
	ShapeId NewPipeline    ( ShapeId const, ShapeId const );

	void CreateNewBranch      ( ShapeId const );
	void CreateNewNeuron      ( MicroMeterPoint const & );
	void CreateNewInputNeuron ( MicroMeterPoint const & );
	void CreateNewOutputNeuron( MicroMeterPoint const & );
	void SplitPipeline        ( ShapeId const, MicroMeterPoint const & );
	void Connect              ( );

	void AddIncomming( ShapeId const, ShapeId const );
	void AddOutgoing ( ShapeId const, ShapeId const );

	void RemovePipeline( ShapeId const );

	void HighlightShape     ( ShapeId const );
	void SuperHighlightShape( ShapeId const );

	void Apply2AllShapes      ( std::function<void(Shape       &)> const & ) const;
	void Apply2AllNeurons     ( std::function<void(Neuron      &)> const & ) const;
	void Apply2AllBaseKnots   ( std::function<void(BaseKnot    &)> const & ) const;
	void Apply2AllPipelines   ( std::function<void(Pipeline    &)> const & ) const;
	void Apply2AllInputNeurons( std::function<void(InputNeuron &)> const & ) const;

	void Apply2AllParameters( std::function<void(tParameter const &)> const & ) const;

	void RecalcPipelines( );

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	void  const SetParameter( tParameter const,	float const, Shape * const = nullptr );

private:
			  
	// modal data
	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;

    // used by editor
	ShapeId m_shapeHighlighted;
	ShapeId m_shapeSuperHighlighted;

	// parameters
	float        m_dampingFactor;     // signal loss per um  
    mV           m_threshold;
    mV           m_peakVoltage;   
	MicroSecs    m_pulseWidth;   
	MicroSecs    m_refractoryPeriod;
	meterPerSec  m_pulseSpeed;

	// local functions
	void          deleteShape( ShapeId const );
	void          deleteHighlightedShape( );
	void          checkConsistency( Shape * );
	void          checkConsistency( );
	ShapeId const addShape( Shape * );
	void          createAxon( ShapeId const );
};
