// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
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
class ComputeThread;
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

	MicroMeterPoint const GetShapePos   ( ShapeId const id  ) const;
	ShapeType       const GetShapeType  ( ShapeId const id  ) const;
	bool            const HasOutgoing   ( ShapeId const id  ) const;
	bool            const HasIncoming   ( ShapeId const id  ) const;
	bool            const IsValidShapeId( ShapeId const id  ) const { return id.GetValue() < m_Shapes.size(); }

	fMicroSecs      const GetSimulationTime ( ) const { return m_timeStamp; }
	long            const GetSizeOfShapeList( ) const { return CastToLong( m_Shapes.size() ); }
	bool            const HasModelChanged   ( ) const { return m_bUnsavedChanges; }
	bool            const IsInEmphasizeMode ( ) const { return m_bEmphasizeMode; }
	fMicroSecs      const GetTimeResolution ( ) const { return m_usResolution; }
	float           const GetOpacity        ( ) const { return IsInEmphasizeMode() ? 0.5f : 1.0f; }
//	int             const GetNrOfThreads    ( ) const { return m_iNrOfComputeThreads; }
	long            const GetNrOfShapes     ( ) const;

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
		pT->SetId( ShapeId { GetSizeOfShapeList() } );
		m_Shapes.push_back( pT );
		return pT;
	}

	template <typename T>
	void AppendShape( ShapeId const id )
	{
		if ( GetShapeType( id ).IsKnotType() )
		{
			Connect( id, NewShape<T>( GetShapePos( id ) )->GetId() );
		}
	}

	template <typename T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
			{
				bResult = func( static_cast<T &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
				func( static_cast<T &>( * pShape ) );
		}
	}

	//void Apply2AllWithSteps( int const iStart, int const iStep, function<void(Shape &)> const & func ) const
	//{
	//	for ( int i = iStart; i < m_Shapes.size(); i += iStep )
	//	{
	//		if ( m_Shapes[i] != nullptr  )
	//			func( * m_Shapes[i] );
	//	}
	//}

	//void DoPrepare( unsigned int const uiStart )
	//{
	//	unsigned int uiStop { min( uiStart + CHUNK_SIZE, CastToUnsignedInt(m_Shapes.size()) ) };
	//	for ( unsigned int ui = uiStart; ui < uiStop; ++ui )
	//	{
	//		if ( m_Shapes[ui] != nullptr )
	//			m_Shapes[ui]->Prepare();
	//	}
	//}

	//void DoStep( unsigned int const uiStart )
	//{
	//	unsigned int uiStop { min( uiStart + CHUNK_SIZE, CastToUnsignedInt(m_Shapes.size()) ) };
	//	for ( unsigned int ui = uiStart; ui < uiStop; ++ui )
	//	{
	//		if ( m_Shapes[ui] != nullptr )
	//			m_Shapes[ui]->Step();
	//	}
	//}

	template <typename T>
	void Apply2AllInRect( MicroMeterRect const & rect, function<void(T &)> const & func ) const
	{
		for ( auto pShape : m_Shapes )
		{
			if ( 
				  pShape && 
				  T::TypeFits( pShape->GetShapeType() ) &&
				  pShape->IsInRect( rect )
			   )
			{
				func( static_cast<T &>( * pShape ) );
			}
		}
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
	void SetEmphasizeMode( bool const );
	void ResetModel( );
	void ClearModel( );
	void ModelSaved  ( ) const;
	void ModelChanged( ) const;
	void SetPulseRate( ShapeId    const, float const );
	void SetParameter( tParameter const, float const );
	void SetNrOfShapes( long lNrOfShapes ) { m_Shapes.resize( lNrOfShapes ); }

	MicroMeterRect GetEnclosingRect( );

	void CheckConsistency() { Apply2All<Shape>( [&]( Shape & shape ) { checkConsistency( & shape ); } ); }

	void AddParameterObserver( ObserverInterface * pObs ) { m_paramObservable.RegisterObserver( pObs ); }

	virtual void Compute( );

	//static int const CHUNK_SIZE { 100 };

	//void StartWork( )
	//{
	//	WaitForSingleObject( m_mutex, INFINITE );
	//	m_uiChunksDone = 0;
	//	ReleaseMutex( m_mutex );
	//}

	//void ChunkDone( )
	//{
	//	WaitForSingleObject( m_mutex, INFINITE );
	//	++m_uiChunksDone;
	//	ReleaseMutex( m_mutex );
	//}

	//bool GetNextChunk( unsigned int & uiNextIndex )
	//{
	//	bool bRes;
	//	WaitForSingleObject( m_mutex, INFINITE );
	//	uiNextIndex = m_uiChunksDone * CHUNK_SIZE;
	//	bRes = m_uiChunksDone < m_uiNrOfChunks;
	//	ReleaseMutex( m_mutex );
	//	return bRes;
	//}

private:
	//unsigned int m_uiNrOfComputeThreads { 0 };

	//SRWLOCK m_SRWLockStartWorking { SRWLOCK_INIT };
	//SRWLOCK m_SRWLockStarted      { SRWLOCK_INIT };
	//SRWLOCK m_SRWLockFinished     { SRWLOCK_INIT };

	//vector< ComputeThread * > m_ComputeThreads;

	//unsigned int    m_uiNrOfChunks   { 0 };
	//unsigned int    m_uiChunksDone   { 0 };
	//unsigned int    m_uiNextFreeChunk{ 0 };
	//HANDLE m_mutex;

	Observable      m_paramObservable { };
	vector<Shape *> m_Shapes          { };
	fMicroSecs      m_timeStamp       { 0._MicroSecs };
	bool            m_bEmphasizeMode  { false };
	mutable bool    m_bUnsavedChanges { false };  // can be changed in const functions

	// parameters
    mV          m_threshold    { 20._mV          };
	mV          m_peakVoltage  { 10._mV          };   
	fMicroSecs  m_pulseWidth   { 2000._MicroSecs };   
	fMicroSecs  m_refractPeriod{ 500._MicroSecs  };
	meterPerSec m_pulseSpeed   { 0.1_meterPerSec };
	fMicroSecs  m_usResolution { 100._MicroSecs  };

	// local functions

	Shape const   * findShapeAt( MicroMeterPoint const, function<bool(Shape const &)> const & ) const;
	void            checkConsistency( Shape * );
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

MicroMeterRect GetEnclosingRect( vector<Shape*> const & );
