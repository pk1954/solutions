// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "synchapi.h"
#include "util.h"
#include "MoreTypes.h"
#include "Observable.h"
#include "Segment.h"
#include "tParameter.h"
#include "tHighlightType.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipeline.h"

class ObserverInterface;
class EventInterface;
class InputNeuron;

class NNetModel
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
	D2D1::ColorF    const GetFrameColor( tHighlightType const )        const;
	bool            const HasModelChanged( )                           const { return m_bUnsavedChanges; }
	MicroSecs       const GetTimeResolution( )                         const { return m_usResolution; }
	
	BaseKnot * const GetStartKnotPtr( ShapeId const idPipeline ) const 
	{ 
		return GetConstTypedShape<Pipeline>( idPipeline )->GetStartKnotPtr(); 
	}

	BaseKnot * const GetEndKnotPtr( ShapeId const idPipeline ) const 
	{ 
		return GetConstTypedShape<Pipeline>( idPipeline )->GetEndKnotPtr(); 
	}

	size_t const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t const GetNrOfIncomingConnections( ShapeId const ) const; 

	Shape const * FindShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;

	float const GetPulseRate     ( InputNeuron const * ) const;
	float const GetParameterValue( tParameter  const   ) const;

	bool const ConnectsTo( ShapeId const, ShapeId const ) const;

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> 
	T * const NewShape( MicroMeterPoint const & pos ) 
	{ 
		auto pT { new T( pos ) };
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
	bool Apply2All( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		EnterCritSect( );
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
			{
				bResult = func( static_cast<T &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		LeaveCritSect( );
		return bResult;
	}

	void CreateInitialShapes();
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
	void Disconnect( ShapeId const );
	void RemoveShape( ShapeId const );
	void RecalcAllShapes( );
	void ResetModel( );
	void ClearModel( );
	void ModelSaved( ) { m_bUnsavedChanges = false; }
	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );
	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	void EnterCritSect() const { EnterCriticalSection( & m_criticalSection ); }
	void LeaveCritSect() const { LeaveCriticalSection( & m_criticalSection ); }

	void CheckConsistency() { Apply2All<Shape>( [&]( Shape & shape ) { checkConsistency( & shape ); return false; } ); }

	void AddParameterObserver( ObserverInterface * pObs ) { m_parameterObservable.RegisterObserver( pObs ); }

	virtual void Compute( );

	float GetOpacity( ) const { return m_fOpacity; };
	void  SetOpaqueMode( bool fMode ) { m_fOpacity = fMode ? 0.5f : 1.0f; };

private:

	static CRITICAL_SECTION m_criticalSection;
	static bool             m_bCritSectReady;

	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;
	bool            m_bUnsavedChanges;

	// parameters
    mV          m_threshold;
    mV          m_peakVoltage;   
	MicroSecs   m_pulseWidth;   
	MicroSecs   m_refractPeriod;
	meterPerSec m_pulseSpeed;
	MicroSecs   m_usResolution;
	float       m_fOpacity;

	Observable  m_parameterObservable;

	// local functions

	Shape const   * findShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	void            checkConsistency( Shape const * ) const;
	MicroMeterPoint orthoVector( ShapeId const ) const;
	void            disconnectBaseKnot( BaseKnot * const );
	void            deletePipeline( ShapeId const );
	void            insertBaseKnot( Pipeline * const, BaseKnot * const );
	void            deleteShape( ShapeId const );
	bool const      isConnectedTo( ShapeId, ShapeId ) const;
	bool const      isConnectedToPipeline( ShapeId const, Pipeline const * const ) const;
	bool            connectIncoming( Pipeline * const, BaseKnot * const );
	bool            connectOutgoing( Pipeline * const, BaseKnot * const );
};
