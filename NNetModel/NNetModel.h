// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "tHighlightType.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

enum class tParameter
{
	pulseSpeed,
	pulseWidth,
	signalLoss,
	threshold,
	peakVoltage,
	refractoryPeriod,
	tParameterLastGlobal = refractoryPeriod,
	pulseRate,
	tParameterLast = pulseRate
};

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( );

	// readOnly functions

	Shape * GetShape( ShapeId const id )
	{
		return ( IsDefined( id ) && IsValidShapeId( id ) )
			   ? m_Shapes[ id.GetValue() ]
			   : nullptr;
	}

	Shape const * GetConstShape( ShapeId const id ) const
	{
		return ( IsDefined( id ) && IsValidShapeId( id ) )
			   ? m_Shapes[ id.GetValue() ]
			   : nullptr;
	}

	template <typename T>
	T * GetTypedShape( ShapeId const id ) 
	{
		Shape * pShape = GetShape( id );
		return ( pShape && T::TypeFits( pShape->GetShapeType() ) ) 
			? static_cast<T *>( pShape )
			: nullptr;
	}

	template <typename T>
	T const * GetConstTypedShape( ShapeId const id ) const
	{
		Shape const * pShape = GetConstShape( id );
		return ( pShape && T::TypeFits( pShape->GetShapeType() ) ) 
			? static_cast<T const *>( pShape )
			: nullptr;
	}

	template <typename T> bool IsType( ShapeId const id ) const { return T::TypeFits( GetShapeType( id ) ); }

	MicroMeterPoint const GetShapePos       ( ShapeId const   id     ) const;
	tShapeType      const GetShapeType      ( ShapeId const   id     ) const;
	bool            const HasAxon           ( ShapeId const   id     ) const;
	bool            const IsValidShapeId    ( ShapeId const   id     ) const { return id.GetValue() < m_Shapes.size(); }
	ShapeId         const GetId             ( Shape   const * pShape ) const { return pShape ? pShape->GetId( ) : NO_SHAPE; }
	MicroSecs       const GetSimulationTime( )                         const { return m_timeStamp; }
	long            const GetSizeOfShapeList( )                        const { return CastToLong( m_Shapes.size() ); }
	long            const GetNrOfShapes( )                             const;
	COLORREF        const GetFrameColor( tHighlightType const )        const;
	wchar_t const * const GetParameterName( tParameter const )         const;
	wchar_t const * const GetParameterUnit( tParameter const )         const;

	Shape const * FindShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeAt( MicroMeterPoint const ) const;

	float const GetPulseRate     ( Shape const * )    const;
	float const GetParameterValue( tParameter const ) const;

	void CheckConsistency( Shape const * ) const;

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> ShapeId NewShape( MicroMeterPoint const & pos ) { return addShape( new T( this, pos ) ); }

	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }

	ShapeId NewPipeline( ShapeId const, ShapeId const );

	void ConnectPipeline( Pipeline *, ShapeId const, ShapeId const, ShapeId const );

	ShapeId const SplitPipeline( ShapeId const, MicroMeterPoint const & );
	ShapeId const InsertNeuron ( ShapeId const, MicroMeterPoint const & );

	void AddNeuron      ( ShapeId const, MicroMeterPoint const & );
	void AddInputNeuron ( ShapeId const, MicroMeterPoint const & );
	void AddOutgoing    ( ShapeId const, MicroMeterPoint const & );
	void AddIncoming    ( ShapeId const, MicroMeterPoint const & );
	void MoveShape      ( ShapeId const, MicroMeterPoint const & );
	void Connect( ShapeId const, ShapeId const );

	void RemoveShape( ShapeId const );

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
				func( static_cast<T &>( * pShape ) );
		}
	}

	void Apply2GlobalParameters( function<void(tParameter const &)> const & ) const;
	void Apply2AllParameters   ( function<void(tParameter const &)> const & ) const;

	void RecalcPipelines( );

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );

	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

private:

	// model data
	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;

	// parameters
	float        m_signalLoss;     // signal loss per um  
    mV           m_threshold;
    mV           m_peakVoltage;   
	MicroSecs    m_pulseWidth;   
	MicroSecs    m_refractoryPeriod;
	meterPerSec  m_pulseSpeed;

	// local functions
	void            deleteBaseKnot( ShapeId const );
	void            deletePipeline( ShapeId const );
	void            insertNewBaseKnot( ShapeId const, BaseKnot * const );
	void            checkConsistency( );
	ShapeId const   addShape( Shape * );
	bool            areConnected( ShapeId const, ShapeId const );
	MicroMeterPoint orthoVector( ShapeId const ) const;
};
