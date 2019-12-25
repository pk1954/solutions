// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "synchapi.h"
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
	bool            const HasOutgoing       ( ShapeId const   id     ) const;
	bool            const HasIncoming       ( ShapeId const   id     ) const;
	bool            const IsValidShapeId    ( ShapeId const   id     ) const { return id.GetValue() < m_Shapes.size(); }
	ShapeId         const GetId             ( Shape   const * pShape ) const { return pShape ? pShape->GetId( ) : NO_SHAPE; }
	MicroSecs       const GetSimulationTime( )                         const { return m_timeStamp; }
	long            const GetSizeOfShapeList( )                        const { return CastToLong( m_Shapes.size() ); }
	long            const GetNrOfShapes( )                             const;
	COLORREF        const GetFrameColor( tHighlightType const )        const;
	wchar_t const * const GetParameterName( tParameter const )         const;
	wchar_t const * const GetParameterUnit( tParameter const )         const;
	bool            const HasModelChanged( )                           const { return m_bUnsavedChanges; }

	size_t const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t const GetNrOfIncomingConnections( ShapeId const ) const; 

	Shape const * FindShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeAt( MicroMeterPoint const ) const;

	float const GetPulseRate     ( Shape const * )    const;
	float const GetParameterValue( tParameter const ) const;

	bool const ConnectsTo( ShapeId const, ShapeId const ) const;

	bool const IsConnectedTo( ShapeId const id, Pipeline const * const pPipeline ) const
	{
		return (id == pPipeline->GetStartKnot()) || (id == pPipeline->GetEndKnot());
	}

	bool const IsConnectedTo( ShapeId id1, ShapeId id2 ) const
	{
		if ( IsType<Pipeline>( id1 ) )
			return IsConnectedTo( id2, GetConstTypedShape<Pipeline>( id1 ) );
		if ( IsType<Pipeline>( id2 ) )
			return IsConnectedTo( id1, GetConstTypedShape<Pipeline>( id2 ) );
		else
			return false;
	}

	void CheckConsistency( Shape const * ) const;

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> 
	ShapeId NewShape( MicroMeterPoint const & pos ) 
	{ 
		return addShape( new T( this, pos ) ); 
	}

	template <typename T>
	void AppendShape( ShapeId const id )
	{
		if ( GetShapeType( id ) == tShapeType::knot )
		{
			Connect( id, NewShape<T>( GetShapePos( id ) ) );
		}
	}

	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }

	ShapeId NewPipeline( ShapeId const, ShapeId const );

	void ConnectPipeline( Pipeline *, ShapeId const, ShapeId const, ShapeId const );

	ShapeId const InsertNeuron ( ShapeId const, MicroMeterPoint const & );

	void AddOutgoing( ShapeId const, MicroMeterPoint const & );
	void AddIncoming( ShapeId const, MicroMeterPoint const & );
	void MoveShape  ( ShapeId const, MicroMeterPoint const & );

	void Connect( ShapeId const, ShapeId const );

	void RemoveShape( ShapeId const );

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		EnterCriticalSection( & m_criticalSection );
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
				func( static_cast<T &>( * pShape ) );
		}
		LeaveCriticalSection( & m_criticalSection );
	}

	void Apply2GlobalParameters( function<void(tParameter const &)> const & ) const;
	void Apply2AllParameters   ( function<void(tParameter const &)> const & ) const;

	void RecalcPipelines( );

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	void ResetModel( );
	void ModelSaved( ) { m_bUnsavedChanges = false; }

	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );

	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

private:

	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;
	bool            m_bUnsavedChanges;

	static CRITICAL_SECTION m_criticalSection;
	static bool             m_bCritSectReady;

	// parameters
	float       m_signalLoss;     // signal loss per um  
    mV          m_threshold;
    mV          m_peakVoltage;   
	MicroSecs   m_pulseWidth;   
	MicroSecs   m_refractoryPeriod;
	meterPerSec m_pulseSpeed;

	// local functions
	void deleteShape( ShapeId const id )
	{
		EnterCriticalSection( & m_criticalSection );
		delete m_Shapes[ id.GetValue() ];
		m_Shapes[ id.GetValue() ] = nullptr;
		LeaveCriticalSection( & m_criticalSection );
	}

	void            createInitialShapes();
	void            deleteBaseKnot( ShapeId const );
	void            deletePipeline( ShapeId const );
	void            insertNewBaseKnot( ShapeId const, BaseKnot * const );
	void            checkConsistency( );
	ShapeId const   addShape( Shape * );
	bool            areConnected( ShapeId const, ShapeId const );
	MicroMeterPoint orthoVector( ShapeId const ) const;
	ShapeId const   splitPipeline( ShapeId const, MicroMeterPoint const & );
};
