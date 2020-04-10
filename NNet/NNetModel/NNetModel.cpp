// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "ComputeThread.h"
#include "NNetModel.h"

#ifndef NDEBUG
	#define CHECK_CONSISTENCY CheckConsistency()
#else
	#define CHECK_CONSISTENCY
#endif

using namespace std::chrono;
using std::unordered_map;

NNetModel::NNetModel( )
{					
	Shape::SetModel( this );
}

NNetModel::~NNetModel( )
{
	m_paramObservable.UnregisterAllObservers();
}

void NNetModel::CreateInitialShapes( )
{
	InputNeuron * const pInputNeuron { NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	NewPipe( pInputNeuron, pNeuron );
}

void NNetModel::ModelSaved( ) const 
{ 
	// reset flag in app title
	m_bUnsavedChanges = false; 
}

void NNetModel::ModelChanged( ) const 
{ 
	if ( ! m_bUnsavedChanges )
	{
		// set flag in app title
	}
	m_bUnsavedChanges = true;  
}

void NNetModel::RecalcAllShapes( ) 
{ 
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Recalc( ); } );
} 

void NNetModel::SetEmphasizeMode( bool const bMode ) 
{ 
	if ( m_bEmphasizeMode && (bMode == false) )
		Apply2All<Shape>( [&]( Shape & shape ) { shape.Emphasize( false ); } );
	m_bEmphasizeMode = bMode;
} 

long const NNetModel::GetNrOfShapes( ) const
{
	long lCounter = 0;
	Apply2All<Shape>( [&]( Shape & shape ) { ++ lCounter; } );
	return lCounter;
}

bool const NNetModel::ConnectsTo( ShapeId const idSrc, ShapeId const idDst ) const
{
	if ( idSrc == idDst )
		return false;

	if ( isConnectedTo( idSrc, idDst ) )  // if already connected we cannot connect again
		return false;

	ShapeType const typeSrc { GetShapeType( idSrc ) };

	if ( typeSrc.IsPipeType() )           // only BaseKnots can be connected
		return false;                         // to other shapes

	if ( typeSrc.IsInputNeuronType() )        // input neuron cannot connect to anything
		return false;                         // try the other way round

	ShapeType const typeDst { GetShapeType( idDst ) };

	if ( typeDst.IsPipeType( ) )
	{
		if ( typeSrc.IsInputNeuronType() )   // cannot insert input neuron
			return false;                           // into Pipe
	}
	else if ( typeDst.IsAnyNeuronType() )
	{
		if ( typeSrc.IsAnyNeuronType() )  // neurons cannot connect directly 
			return false;                  // to other neurons

		if ( typeDst.IsInputNeuronType() && HasIncoming( idSrc ) )  // cannot connect incoming dendrite
			return false;                                           // to input neuron

		if ( GetNrOfOutgoingConnections( idSrc ) + GetNrOfOutgoingConnections( idDst ) > 1 ) // neurons can not not have 
			return false;                                                                    // more than one axon
	}

	return true;
}

void NNetModel::removeShape( Shape const * pShape )
{
	if ( pShape )
	{
		ShapeId id { pShape->GetId( ) };
		switch ( pShape->GetShapeType( ).GetValue() )
		{
		case ShapeType::Value::pipe:
			deletePipe( id );
			break;

		case ShapeType::Value::inputNeuron:
		case ShapeType::Value::neuron:
			disconnectBaseKnot( GetShapePtr<BaseKnot *>( id ) );
			deleteShape( id );
			break;

		case ShapeType::Value::knot:
		default:
			break;
		}
		ModelChanged( );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::RemoveShape( ShapeId const idShapeToBeDeleted )
{
	removeShape( GetShape( idShapeToBeDeleted ) );
}

void NNetModel::Disconnect( ShapeId const id )
{
	if ( BaseKnot * pBaseKnot { GetShapePtr<BaseKnot *>( id ) } )
	{
		disconnectBaseKnot( pBaseKnot );
		assert( pBaseKnot->IsOrphan( ) );
		if ( pBaseKnot->GetShapeType( ).IsKnotType() )
			deleteShape( id );
	}
	CHECK_CONSISTENCY;
}

void NNetModel::Convert2Neuron( ShapeId const idInputNeuron )
{
	if ( InputNeuron * pInputNeuron { GetShapePtr<InputNeuron *>( idInputNeuron ) } )
	{
		ShapeId         const idAxon  { pInputNeuron->GetAxonId( ) };
		MicroMeterPoint const pos     { pInputNeuron->GetPosition( ) };
		Neuron        * const pNeuron { NewShape<Neuron>( pos ) };
		RemoveShape( idInputNeuron );
		if ( idAxon != NO_SHAPE )
			Connect( GetStartKnotId( idAxon ), pNeuron->GetId()  );
	}
	CHECK_CONSISTENCY;
}

void NNetModel::Convert2InputNeuron( ShapeId const idNeuron )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( idNeuron ) } )
	{
		ShapeId         const idAxon  { pNeuron->GetAxonId( ) };
		MicroMeterPoint const pos     { pNeuron->GetPosition( ) };
		InputNeuron   * const pInputNeuron { NewShape<InputNeuron>( pos ) };
		RemoveShape( idNeuron );
		if ( idAxon != NO_SHAPE )
			Connect( GetStartKnotId( idAxon ), pInputNeuron->GetId()  );
	}
	CHECK_CONSISTENCY;
}

float const NNetModel::GetPulseRate( InputNeuron const * pInputNeuron ) const
{
	return pInputNeuron ? pInputNeuron->GetPulseFrequency().GetValue() : 0.0f;
}

float const NNetModel::GetParameterValue( tParameter const param ) const
{
	switch ( param )
	{
		case tParameter::pulseSpeed:	 return m_pulseSpeed.GetValue();
		case tParameter::pulseWidth:	 return m_pulseWidth.GetValue();
		case tParameter::threshold:  	 return m_threshold.GetValue();
		case tParameter::peakVoltage:	 return m_peakVoltage.GetValue();
		case tParameter::refractPeriod:  return m_refractPeriod.GetValue();
		case tParameter::timeResolution: return m_usResolution.GetValue();
		case tParameter::signalLoss:     return 0.0f;  // not used, only for compaitibility reasons
		default: assert( false );
	}
	return 0.f;
}

void NNetModel::SetPulseRate( ShapeId const id, float const fNewValue )
{
	ModelChanged( );
	InputNeuron * const pInputNeuron { GetShapePtr<InputNeuron *>( id ) };
	if ( pInputNeuron )
		pInputNeuron->SetPulseFrequency( static_cast< fHertz >( fNewValue ) );
}

void NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	ModelChanged( );
	switch ( param )
	{
		case tParameter::pulseSpeed:	 m_pulseSpeed    = static_cast< meterPerSec >( fNewValue ); break;
		case tParameter::pulseWidth:	 m_pulseWidth    = static_cast< fMicroSecs  >( fNewValue ); break;
		case tParameter::threshold:		 m_threshold     = static_cast< mV >         ( fNewValue ); break;
		case tParameter::peakVoltage:	 m_peakVoltage   = static_cast< mV >         ( fNewValue ); break;
		case tParameter::refractPeriod:	 m_refractPeriod = static_cast< fMicroSecs  >( fNewValue ); break;
		case tParameter::timeResolution: 
			m_usResolution  = static_cast< fMicroSecs  >( fNewValue ); 
			break;
		case tParameter::signalLoss: /* not used, only for compaitibility reasons */                break;
		default: assert( false );
	}
	RecalcAllShapes( );
	m_paramObservable.NotifyAll( FALSE );
}

MicroMeterPoint const NNetModel::GetShapePos( ShapeId const id ) const 
{
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetPosition();
	return NP_NULL;
}

ShapeType const NNetModel::GetShapeType( ShapeId const id ) const 
{
	if ( Shape const * pShape = GetConstShape( id ) )
		return pShape->GetShapeType();
	return ShapeType::Value::undefined;
}

bool const NNetModel::HasOutgoing( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->HasOutgoing( );
	return false;
}

bool const NNetModel::HasIncoming( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->HasIncoming( );
	return false;
}

size_t const NNetModel::GetNrOfOutgoingConnections( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetNrOfOutgoingConnections( );
	return false;
}

size_t const NNetModel::GetNrOfIncomingConnections( ShapeId const id ) const 
{ 
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetNrOfIncomingConnections( );
	return false;
}

void NNetModel::Connect( ShapeId const idSrc, ShapeId const idDst )  // merge src shape into dst shape
{
	BaseKnot * pSrc = GetShapePtr<BaseKnot *>( idSrc );
	if ( GetShapeType( idDst ).IsPipeType() )   // connect baseknot to pipe
	{
		insertBaseKnot( GetShapePtr<Pipe *>( idDst ), pSrc );
	}
	else  // connect baseknot to baseknot
	{
		BaseKnot * pDst = GetShapePtr<BaseKnot *>( idDst );
		if ( pSrc && pDst )
		{
			pSrc->Apply2AllInPipes ( [&]( Pipe * const pipe ) { connectIncoming( pipe, pDst ); } );
			pSrc->Apply2AllOutPipes( [&]( Pipe * const pipe ) { connectOutgoing( pipe, pDst ); } );
			if ( pSrc->IsKnot() )
				deleteShape( idSrc );
		}
	}
	ModelChanged( );
	CHECK_CONSISTENCY;
}

void NNetModel::NewPipe( BaseKnot * const pStart, BaseKnot * const pEnd )
{
	if ( pStart && pEnd )
	{
		ModelChanged( );
		Pipe * const pPipe { NewShape<Pipe>( NP_NULL ) };
		connectOutgoing( pPipe, pStart );
		connectIncoming( pPipe, pEnd );
		CHECK_CONSISTENCY;
	}
}

void NNetModel::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	ModelChanged( );
	switch ( GetShapeType( id ).GetValue() )
	{
	case ShapeType::Value::pipe:
	{
		GetStartKnotPtr( id )->MoveShape( delta );
		GetEndKnotPtr  ( id )->MoveShape( delta );
		break;
	}

	case ShapeType::Value::inputNeuron:
	case ShapeType::Value::neuron:
	case ShapeType::Value::knot:
		GetShapePtr<BaseKnot *>( id )->MoveShape( delta );
		break;

	default:
		break;
	}
	CHECK_CONSISTENCY;
}

Neuron * const NNetModel::InsertNeuron( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Neuron * pNeuron { nullptr };
	if ( IsDefined( idPipe ) )
	{
		ModelChanged( );
		pNeuron = NewShape<Neuron>( splitPoint );
		insertBaseKnot( GetShapePtr<Pipe *>(idPipe), pNeuron );
		CHECK_CONSISTENCY;
	}
	return pNeuron;
}

Knot * const NNetModel::InsertKnot( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Knot * pKnot { nullptr };
	if ( IsDefined( idPipe ) )
	{
		ModelChanged( );
		pKnot = NewShape<Knot>( splitPoint );
		insertBaseKnot( GetShapePtr<Pipe *>(idPipe), pKnot );
		CHECK_CONSISTENCY;
	}
	return pKnot;
}

void NNetModel::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( InsertKnot( id, pos ), NewShape<Knot>( pos + orthoVector( id ) ) );
}

void NNetModel::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( NewShape<Knot>( pos - orthoVector( id ) ), InsertKnot( id, pos ) );
}

void NNetModel::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( GetShapePtr<BaseKnot *>(id), NewShape<Knot>( pos + STD_OFFSET) );
}

void NNetModel::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( NewShape<Knot>( pos - STD_OFFSET ), GetShapePtr<BaseKnot *>( id ) );
}

void NNetModel::Compute( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Step( ); } );

	m_timeStamp += GetTimeResolution( );
}

void NNetModel::ResetModel( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { delete & shape; } );
	m_Shapes.clear();
	ModelChanged( );
}

void NNetModel::ClearModel( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Clear( ); } );
}

Shape const * NNetModel::FindShapeAt
( 
	MicroMeterPoint               const   pnt, 
	function<bool(Shape const &)> const & crit 
) const
{	
	Shape const * pRes { nullptr };

	if ( ! pRes )  // first test all neurons and input neurons
		pRes = findShapeAt( pnt, [&]( Shape const & s ) { return s.IsAnyNeuron( ) && crit( s ); } );

	if ( ! pRes )   // if nothing found, test knot shapes
		pRes = findShapeAt( pnt, [&]( Shape const & s ) { return s.IsKnot     ( ) && crit( s ); } ); 	

	if ( ! pRes ) // if nothing found, try pipes
		pRes = findShapeAt( pnt, [&]( Shape const & s ) { return s.IsPipe     ( ) && crit( s ); } );

	return pRes;
}

/////////////////// local functions ///////////////////////////////////////////////

void NNetModel::checkConsistency( Shape * pShape )
{
	ShapeType type = pShape->GetShapeType();

	if ( type.IsBaseKnotType() )
		static_cast<BaseKnot &>( * pShape ).Apply2AllConnectedPipes
		( 
			[&]( Pipe const * const pipe ) 
			{ 
				assert( pipe->IsPipe() ); 
				return false; 
			} 
	    );

	switch ( type.GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		assert( ! static_cast<InputNeuron const *>( pShape )->HasIncoming() );
		break;

	case ShapeType::Value::knot:
		break;

	case ShapeType::Value::neuron:
		break;

	case ShapeType::Value::pipe:
		{
			Pipe const * pPipe { static_cast<Pipe const *>( pShape ) };
			assert( pPipe );
			if ( Shape const * const pStart { pPipe->GetStartKnotPtr() } )
				assert( pStart->IsBaseKnot() );
			if ( Shape const * const pEnd   { pPipe->GetEndKnotPtr() } )
				assert( pEnd->IsBaseKnot() );
			assert( pPipe->GetStartKnotId() != pPipe->GetEndKnotId() );
			break;
		}

	case ShapeType::Value::undefined:
		assert( false );
		break;

	default:
		assert( false );
	}
}

bool const NNetModel::isConnectedToPipe( ShapeId const id, Pipe const * const pPipe ) const
{
	return (id == pPipe->GetStartKnotId()) || (id == pPipe->GetEndKnotId());
}

bool const NNetModel::isConnectedTo( ShapeId id1, ShapeId id2 ) const
{
	if ( IsOfType<Pipe>( id1 ) )
		return isConnectedToPipe( id2, GetShapeConstPtr<Pipe const *>( id1 ) );
	if ( IsOfType<Pipe>( id2 ) )
		return isConnectedToPipe( id1, GetShapeConstPtr<Pipe const *>( id2 ) );
	else
		return false;
}

void NNetModel::deleteShape( ShapeId const id )
{
	if ( IsDefined( id ) )
	{
		delete m_Shapes[ id.GetValue() ];
		m_Shapes[ id.GetValue() ] = nullptr;
	}
}

void NNetModel::connectIncoming
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	if ( pPipe && pEndPoint )
	{
		pEndPoint->AddIncoming( pPipe );
		pPipe->SetEndKnot ( pEndPoint );
	}
}

void NNetModel::connectOutgoing
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	if ( pPipe && pStartPoint )
	{
		pStartPoint->AddOutgoing( pPipe );
		pPipe->SetStartKnot ( pStartPoint );
	}
}

void NNetModel::disconnectBaseKnot( BaseKnot * const pBaseKnot ) // disconnects only, shape remains
{
	if ( pBaseKnot )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		pBaseKnot->Apply2AllInPipes
		( 
			[&]( Pipe * const pipe ) // every incoming Pipe needs a new end knot
			{ 
				connectIncoming( pipe, NewShape<Knot>( umPos ) );
				pipe->DislocateEndPoint( );
			} 
		);
		pBaseKnot->ClearIncoming();
		pBaseKnot->Apply2AllOutPipes
		( 
			[&]( Pipe * const pipe ) // every outgoing Pipe needs a new start knot
			{ 
				connectOutgoing( pipe, NewShape<Knot>( umPos ) );
				pipe->DislocateStartPoint( );
			} 
		);
		pBaseKnot->ClearOutgoing();
	}
}

void NNetModel::deletePipe( ShapeId const id )
{
	if ( IsDefined( id ) )
	{
		Pipe * pPipeToBeDeleted { GetShapePtr<Pipe *>( id ) };

		BaseKnot * pStartKnot { pPipeToBeDeleted->GetStartKnotPtr() };
		pStartKnot->RemoveOutgoing( pPipeToBeDeleted );
		if ( pStartKnot->IsOrphanedKnot( ) && ( pStartKnot->IsKnot() ) )
			deleteShape( pStartKnot->GetId() );

		BaseKnot * pEndKnot { pPipeToBeDeleted->GetEndKnotPtr() };
		pEndKnot->RemoveIncoming( pPipeToBeDeleted );
		if ( pEndKnot->IsOrphanedKnot( ) && ( pEndKnot->IsKnot() ) )
			deleteShape( pEndKnot->GetId() );

		deleteShape( id );
	}
}

void NNetModel::insertBaseKnot( Pipe * const pPipe, BaseKnot * const pBaseKnot)
{
	if ( pPipe && pBaseKnot )
	{
		BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr( ) };
		NewPipe( pStartKnot, pBaseKnot );
		pStartKnot->RemoveOutgoing( pPipe );
		connectOutgoing( pPipe, pBaseKnot );
	}
}

MicroMeterPoint NNetModel::orthoVector( ShapeId const idPipe ) const
{
	return OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
}

Shape const * NNetModel::findShapeAt
( 
	MicroMeterPoint               const   pnt, 
	function<bool(Shape const &)> const & crit 
) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape;
	};
	return nullptr;
}

MicroMeterRect NNetModel::GetEnclosingRect( )
{
	return ::GetEnclosingRect( m_Shapes );
}

/////////////////// nonmember functions ///////////////////////////////////////////////

MicroMeterRect GetEnclosingRect( vector<Shape*> const & shapeVector )
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for ( const auto & pShape : shapeVector )
	{
		if ( pShape )
		{
			if ( pShape->IsBaseKnot() )
			{
				rect.Expand( Cast2BaseKnot( pShape )->GetPosition() );
			}
			else if ( pShape->IsPipe() )
			{
				rect.Expand( Cast2Pipe( pShape )->GetStartPoint() );
				rect.Expand( Cast2Pipe( pShape )->GetEndPoint() );
			}
			else
			{
				assert( false );
			}
		}
	}
	return rect;
}
