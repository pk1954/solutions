// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
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

	template <typename T> bool IsType( ShapeId const id ) const { return T::TypeFits( GetConstShape( id )->GetShapeType() ); }

	MicroSecs       const GetSimulationTime( )                         const { return m_timeStamp; }
	ShapeId         const GetHighlightedShapeId( )                     const { return m_shapeHighlighted; }
	ShapeId         const GetSuperHighlightedShapeId( )                const { return m_shapeSuperHighlighted; }
	ShapeId         const GetId             ( Shape   const * pShape ) const { return pShape ? pShape->GetId( ) : NO_SHAPE; }
	bool            const IsHighlighted     ( Shape   const & shape  ) const { return shape.GetId() == m_shapeHighlighted; }
	bool            const IsSuperHighlighted( Shape   const & shape  ) const { return shape.GetId() == m_shapeSuperHighlighted; }
	bool            const IsValidShapeId    ( ShapeId const   id     ) const { return id.GetValue() < m_Shapes.size(); }
	bool            const HasAxon           ( ShapeId const   id     ) const { return GetConstShape( id )->HasAxon( ); }
	long            const GetSizeOfShapeList( )                        const { return CastToLong( m_Shapes.size() ); }
	long            const GetNrOfShapes( )                             const;
	tShapeType      const GetHighlightedShapeType( )                   const;
	tShapeType      const GetSuperHighlightedShapeType( )              const;
	MicroMeterPoint const GetHighlightedShapePos( )                    const;
	COLORREF        const GetFrameColor( Shape const & )               const;
	wchar_t const * const GetParameterName( tParameter const )         const;
	wchar_t const * const GetParameterUnit( tParameter const )         const;

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;

	float const GetParameterValue( tParameter const, Shape const * = nullptr ) const;
	
	void CheckConsistency( Shape const * ) const;

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> ShapeId NewShape( MicroMeterPoint const & pos ) { return addShape( new T( this, pos ) ); }

	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }

	ShapeId NewPipeline( ShapeId const, ShapeId const );

	void ConnectPipeline( Pipeline *, ShapeId const, ShapeId const, ShapeId const );

	void SplitPipeline  ( MicroMeterPoint const & );
	void InsertNeuron   ( MicroMeterPoint const & );
	void AddNeuron      ( MicroMeterPoint const & );
	void AddInputNeuron ( MicroMeterPoint const & );
	void AddOutgoing    ( MicroMeterPoint const & );
	void AddIncoming    ( MicroMeterPoint const & );
	void MoveShape      ( MicroMeterPoint const & );
	void Connect( );

	void RemoveShape( );

	ShapeId const HighlightShape     ( ShapeId const );
	ShapeId const SuperHighlightShape( ShapeId const );

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

	void  const SetParameter( ShapeId const, tParameter const,	float const );

	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

private:

	// model data
	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;

    // used by editor
	ShapeId m_shapeHighlighted;
	ShapeId m_shapeSuperHighlighted;

	// parameters
	float        m_signalLoss;     // signal loss per um  
    mV           m_threshold;
    mV           m_peakVoltage;   
	MicroSecs    m_pulseWidth;   
	MicroSecs    m_refractoryPeriod;
	meterPerSec  m_pulseSpeed;

	// local functions
	void          deleteBaseKnot( ShapeId const );
	void          deletePipeline( ShapeId const );
	void          insertNewBaseKnot( BaseKnot * const );
	void          checkConsistency( );
	ShapeId const addShape( Shape * );
	bool          areConnected( ShapeId const, ShapeId const );
};
