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
#include "tParameter.h"
#include "tHighlightType.h"
#include "InputNeuron.h"
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
	bool            const HasModelChanged( )                           const { return m_bUnsavedChanges; }
	
	ShapeId const GetStartKnot( ShapeId const idPipeline ) const 
	{ 
		return GetConstTypedShape<Pipeline>( idPipeline )->GetStartKnot(); 
	}

	ShapeId const GetEndKnot( ShapeId const idPipeline ) const 
	{ 
		return GetConstTypedShape<Pipeline>( idPipeline )->GetEndKnot(); 
	}

	size_t const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t const GetNrOfIncomingConnections( ShapeId const ) const; 

	Shape const * FindShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;

	float const GetPulseRate     ( Shape const * )    const;
	float const GetParameterValue( tParameter const ) const;

	bool const ConnectsTo( ShapeId const, ShapeId const ) const;

	bool const IsConnectedTo( ShapeId id1, ShapeId id2 ) const
	{
		if ( IsType<Pipeline>( id1 ) )
			return isConnectedToPipeline( id2, GetConstTypedShape<Pipeline>( id1 ) );
		if ( IsType<Pipeline>( id2 ) )
			return isConnectedToPipeline( id1, GetConstTypedShape<Pipeline>( id2 ) );
		else
			return false;
	}

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> 
	T * const NewShape( MicroMeterPoint const & pos ) 
	{ 
		auto pT { new T( this, pos ) };
		pT->SetId( ShapeId { CastToLong( m_Shapes.size() ) } );
		m_Shapes.push_back( pT );
		return pT;
	}

	template <typename T>
	void AppendShape( ShapeId const id )
	{
		if ( GetShapeType( id ) == tShapeType::knot )
		{
			Connect( id, NewShape<T>( GetShapePos( id ) )->GetId() );
		}
	}

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

	void SetShape( Shape * const pShape, ShapeId const id )	{ m_Shapes[ id.GetValue() ] = pShape; }

	void NewPipeline( BaseKnot * const, BaseKnot * const );

	Knot   * const InsertKnot  ( ShapeId const, MicroMeterPoint const & );
	Neuron * const InsertNeuron( ShapeId const, MicroMeterPoint const & );
	void           MoveShape   ( ShapeId const, MicroMeterPoint const & );

	void AddOutgoing2Knot( ShapeId const, MicroMeterPoint const & );
	void AddIncoming2Knot( ShapeId const, MicroMeterPoint const & );
	void AddOutgoing2Pipe( ShapeId const, MicroMeterPoint const & );
	void AddIncoming2Pipe( ShapeId const, MicroMeterPoint const & );

	void Connect( ShapeId const, ShapeId const );

	void ConnectIncoming
	( 
		Pipeline * const pPipeline, 
		BaseKnot * const pEndPoint
	)
	{
		pEndPoint->AddIncoming( pPipeline->GetId() );
		pPipeline->SetEndKnot( pEndPoint->GetId() );
	}

	void ConnectOutgoing
	( 
		Pipeline * const pPipeline, 
		BaseKnot * const pStartPoint
	)
	{
		pStartPoint->AddOutgoing( pPipeline->GetId() );
		pPipeline->SetStartKnot( pStartPoint->GetId() );
	}

	void Disconnect( ShapeId const );

	void RemoveShape( ShapeId const );

	void RecalcPipelines( );

	void ResetModel( );
	void ModelSaved( ) { m_bUnsavedChanges = false; }

	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );

	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	void EnterCritSect() { EnterCriticalSection( & m_criticalSection ); }
	void LeaveCritSect() { LeaveCriticalSection( & m_criticalSection ); }

	void CheckConsistency() { Apply2All<Shape>( [&]( Shape & shape ) { checkConsistency( & shape ); } ); };

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

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
	MicroSecs   m_refractPeriod;
	meterPerSec m_pulseSpeed;

	// local functions
	void deleteShape( ShapeId const id )
	{
		EnterCriticalSection( & m_criticalSection );
		delete m_Shapes[ id.GetValue() ];
		m_Shapes[ id.GetValue() ] = nullptr;
		LeaveCriticalSection( & m_criticalSection );
	}

	Shape const   * findShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	void            checkConsistency( Shape const * ) const;
	MicroMeterPoint orthoVector( ShapeId const ) const;
	void            createInitialShapes();
	void            disconnectBaseKnot( BaseKnot * const );
	void            deletePipeline( ShapeId const );
	void            insertBaseKnot( Pipeline * const, BaseKnot * const );

	bool const isConnectedToPipeline( ShapeId const id, Pipeline const * const pPipeline ) const
	{
		return (id == pPipeline->GetStartKnot()) || (id == pPipeline->GetEndKnot());
	}

};
