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
#include "NNetModel.h"

using namespace std::chrono;
using std::unordered_map;

void NNetModel::Initialize
(
	Param      * const pParam, 
	Observable * const pStaticModelObservable,
	Observable * const pDynamicModelObservable,
	Observable * const pModelTimeObservable
)
{					
	m_pParam                  = pParam;
	m_pStaticModelObservable  = pStaticModelObservable;
    m_pDynamicModelObservable = pDynamicModelObservable;
	m_pModelTimeObservable    = pModelTimeObservable;
	Shape::SetParam( pParam );
	m_Shapes.reserve( 100000 ); // Dirty trick to avoid reallocation (invalidates iterators)
}                               // TODO: find better solution  XXXXX

void NNetModel::CreateInitialShapes( )
{
	InputNeuron * const pInputNeuron { NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	ShapeId       const idNewPipe    { NewPipe( pInputNeuron, pNeuron  ) };
}

void NNetModel::RecalcAllShapes( ) 
{ 
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Recalc( ); } );
	dynamicModelChanged( );
} 

void NNetModel::RemoveShape( Shape * const pShape )
{
	if ( pShape )
	{
		if ( pShape->IsPipe() )
			deletePipeEndPoints( Cast2Pipe( pShape ) );
		else 
			DisconnectBaseKnot( Cast2BaseKnot( pShape ) );
		RemoveFromShapeList( pShape );
		delete pShape;
	}
}

void NNetModel::ToggleStopOnTrigger( ShapeId const id )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( id ) } )
	{
		pNeuron->StopOnTrigger( tBoolOp::opToggle );
		dynamicModelChanged( );
	}
}

void NNetModel::Disconnect( ShapeId const id )
{
	if ( BaseKnot * pBaseKnot { GetShapePtr<BaseKnot *>( id ) } )
	{
		DisconnectBaseKnot( pBaseKnot );
		if ( pBaseKnot->IsKnot() )
		{
			RemoveFromShapeList( pBaseKnot );
			delete pBaseKnot;
		}
		StaticModelChanged( );
	}
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
			Connect( GetStartKnotId( idAxon ), pNeuron->GetId() );
		StaticModelChanged( );
	}
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
		StaticModelChanged( );
	}
}

void NNetModel::SetPulseRate( ShapeId const id, bool const bDirection )
{
	InputNeuron * const pInputNeuron { GetShapePtr<InputNeuron *>( id ) };
	if ( pInputNeuron )
	{
		static fHertz const INCREMENT { 0.01_fHertz };
		fHertz const fOldValue { pInputNeuron->GetPulseFreq( ) };
		if ( fOldValue.IsNotNull() )
		{
			pInputNeuron->SetPulseFrequency( fOldValue + ( bDirection ? INCREMENT : -INCREMENT ) );
			StaticModelChanged( );
		}
	}
}

void NNetModel::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	InputNeuron * const pInputNeuron { GetShapePtr<InputNeuron *>( id ) };
	if ( pInputNeuron )
	{
		pInputNeuron->SetPulseFrequency( static_cast< fHertz >( fNewValue ) );
		StaticModelChanged( );
	}
}

void NNetModel::setTriggerSound( Neuron * const pNeuron, Hertz const freq, MilliSecs const msec )
{
	if ( pNeuron )
	{
		pNeuron->SetTriggerSoundFrequency( freq );
		pNeuron->SetTriggerSoundDuration ( msec );
		pNeuron->SetTriggerSoundOn( freq != 0_Hertz );
		StaticModelChanged( );
	}
}

void NNetModel::removeTriggerSound( Neuron * const pNeuron )
{
	setTriggerSound( pNeuron, 0_Hertz, 0_MilliSecs );
}

void NNetModel::SetTriggerSound( ShapeId const id, Hertz const freq, MilliSecs const msec )
{
	setTriggerSound( GetShapePtr<Neuron *>( id ), freq, msec );
	StaticModelChanged( );
}

void NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	m_pParam->SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
}

MicroMeterPoint const NNetModel::GetShapePos( ShapeId const id ) const
{
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetPosition();
	return NP_NULL;
}

void NNetModel::Connect( ShapeId const idSrc, ShapeId const idDst )  // merge src shape into dst shape
{
	BaseKnot * pSrc { GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { GetShapePtr<Shape    *>( idDst ) };
	if ( pDst->IsPipe() )   // connect baseknot to pipe
	{
		InsertBaseKnot( GetShapePtr<Pipe *>( idDst ), pSrc );
	}
	else  // connect baseknot to baseknot
	{
		BaseKnot * pDstBaseKnot { static_cast<BaseKnot *>( pDst ) };
		if ( pSrc && pDstBaseKnot )
		{
			pSrc->Apply2AllInPipes ( [&]( Pipe & pipe ) { ConnectIncoming( & pipe, pDstBaseKnot ); pipe.Recalc(); } );
			pSrc->Apply2AllOutPipes( [&]( Pipe & pipe ) { ConnectOutgoing( & pipe, pDstBaseKnot ); pipe.Recalc(); } );
			RemoveFromShapeList( pSrc );
			delete pSrc;
		}
	}
	StaticModelChanged( );
}

ShapeId const NNetModel::NewPipe( BaseKnot * const pStart, BaseKnot * const pEnd )
{
	if ( pStart && pEnd )
	{
		Pipe * const pPipe { NewShape<Pipe>( NP_NULL ) };
		ConnectOutgoing( pPipe, pStart );
		ConnectIncoming( pPipe, pEnd );
		pPipe->Recalc();
		StaticModelChanged( );
		return pPipe->GetId();
	}
	return { NO_SHAPE };
}

void NNetModel::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( Shape * pShape { GetShapePtr<Shape *>( id  ) } )
	{
		pShape->MoveShape( delta );
		StaticModelChanged( );
	}
}

Neuron * const NNetModel::InsertNeuron( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Neuron * pNeuron { nullptr };
	if ( IsDefined( idPipe ) )
	{
		pNeuron = NewShape<Neuron>( splitPoint );
		InsertBaseKnot( GetShapePtr<Pipe *>(idPipe), pNeuron );
		StaticModelChanged( );
	}
	return pNeuron;
}

Knot * const NNetModel::InsertKnot( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Knot * pKnot { nullptr };
	if ( IsDefined( idPipe ) )
	{
		pKnot = NewShape<Knot>( splitPoint );
		InsertBaseKnot( GetShapePtr<Pipe *>(idPipe), pKnot );
		StaticModelChanged( );
	}
	return pKnot;
}

ShapeId const NNetModel::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	return ( IsDefined( id ) )
		   ? NewPipe( InsertKnot( id, pos ), NewShape<Knot>( pos + orthoVector( id ) ) )
		   : NO_SHAPE;
}

ShapeId const NNetModel::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	return ( IsDefined( id ) )
		   ? NewPipe( NewShape<Knot>( pos - orthoVector( id ) ), InsertKnot( id, pos ) )
	       : NO_SHAPE;
}

ShapeId const NNetModel::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	return ( IsDefined( id ) )
		   ? NewPipe( GetShapePtr<BaseKnot *>(id), NewShape<Knot>( pos + STD_OFFSET) )
	       : NO_SHAPE;
}

ShapeId const NNetModel::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	return ( IsDefined( id ) )
		   ? NewPipe( NewShape<Knot>( pos - STD_OFFSET ), GetShapePtr<BaseKnot *>( id ) )
	       : NO_SHAPE;
}

bool NNetModel::Compute( )
{
	bool bStop {false };
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2All<Shape>( [&]( Shape & shape ) { if ( shape.CompStep( ) ) bStop = true; } );
	dynamicModelChanged();
	incTimeStamp( );
	return bStop;
}

void NNetModel::ResetModel( )
{
	for (auto pShape : m_Shapes)
		delete pShape;
	m_Shapes.clear();
	StaticModelChanged( );
}

void NNetModel::RemoveShape( ShapeId const id ) 
{ 
	RemoveShape( GetShape( id ) ); 
	StaticModelChanged( );
}

void NNetModel::DeleteSelection( )
{
	for ( int i = 0; i < m_Shapes.size(); ++i )  // Caution!
	{	                                         // Range based loop does not work here
		Shape * p = m_Shapes.at(i);              // RemoveShape changes the range 
		if ( p &&  p->IsSelected() )             // by creating new shapes!!
			RemoveShape( p ); 
	}
	StaticModelChanged( );
}

void NNetModel::MarkSelection( tBoolOp const op )
{
	Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( op ); } );
	dynamicModelChanged();
}

void NNetModel::MoveSelection( MicroMeterPoint const & delta )
{
	Apply2AllSelected<BaseKnot>
	( 
		[&]( BaseKnot & knot ) 
		{ 
			knot.MoveShape( delta ); 
			knot.Apply2AllConnectedPipes( [&](Pipe & pipe) { pipe.Recalc(); } );
		} 
	);
	StaticModelChanged( );
}

bool NNetModel::isEqual( Shape const & shapeA, Shape const & shapeB ) const
{
	if ( shapeA.GetShapeType().GetValue() != shapeB.GetShapeType().GetValue() )
		return false;

	switch ( shapeA.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		return static_cast<InputNeuron const &>( shapeA ).IsEqual( static_cast<InputNeuron const &>( shapeB ) );

	case ShapeType::Value::knot:
		return static_cast<Knot const &>( shapeA ).IsEqual( static_cast<Knot const &>( shapeB ) );

	case ShapeType::Value::neuron:
		return static_cast<Neuron const &>( shapeA ).IsEqual( static_cast<Neuron const &>( shapeB ) );

	case ShapeType::Value::pipe:
		return static_cast<Pipe const &>( shapeA ).IsEqual( static_cast<Pipe const &>( shapeB ) );

	default:
		assert( false );
		return false;
	}
	return false;
}

Shape * NNetModel::shallowCopy( Shape const & shape ) const
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		return new InputNeuron( static_cast<InputNeuron const &>( shape ) );

	case ShapeType::Value::knot:
		return new Knot( static_cast<Knot const &>( shape ) );

	case ShapeType::Value::neuron:
		return new Neuron( static_cast<Neuron const &>( shape ) );

	case ShapeType::Value::pipe:
		return new Pipe( static_cast<Pipe const &>( shape ) );

	default:
		assert( false );
		return nullptr;
	}
}

void NNetModel::connectToNewShapes( Shape const & shapeSrc, ShapeList & newShapes )
{ 
	Shape & shapeDst { * newShapes.at(shapeSrc.GetId().GetValue()) };
	if ( shapeSrc.IsPipe( ) )
	{
		Pipe const & pipeSrc { static_cast<Pipe const &>( shapeSrc ) };
		Pipe       & pipeDst { static_cast<Pipe       &>( shapeDst ) };
		pipeDst.SetStartKnot( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetStartKnotId().GetValue() )) );
		pipeDst.SetEndKnot  ( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetEndKnotId  ().GetValue() )) );
		pipeDst.Recalc();
	}
	else
	{
		BaseKnot const & baseKnotSrc { static_cast<BaseKnot const &>( shapeSrc ) };
		BaseKnot       & baseKnotDst { static_cast<BaseKnot       &>( shapeDst ) };
		baseKnotDst.ClearOutgoing();
		baseKnotDst.ClearIncoming();
		auto dstFromSrc = [&](Pipe const & pipeSrc){ return static_cast<Pipe *>(newShapes.at(pipeSrc.GetId().GetValue())); };
		baseKnotSrc.Apply2AllOutPipes( [&]( Pipe const & pipeSrc ) { baseKnotDst.AddOutgoing( dstFromSrc( pipeSrc ) ); } );
		baseKnotSrc.Apply2AllInPipes ( [&]( Pipe const & pipeSrc ) { baseKnotDst.AddIncoming( dstFromSrc( pipeSrc ) ); } );
	}
}

void NNetModel::CopySelection( )
{
	// vector like m_Shapes with ptr to copy of shape or nullptr (if original shape is not in selection)
	ShapeList newShapes( m_Shapes.size(), nullptr ); 

	// make shallow copy of all selected shapes
	Apply2AllSelected<Shape>( [&]( Shape const & shape ) { newShapes.at(shape.GetId().GetValue()) = shallowCopy( shape ); } );

	// interconnect new shapes in same way as originals
	Apply2AllSelected<Shape>( [&]( Shape const & shape ) { connectToNewShapes( shape, newShapes ); } );

	// Deselect original shapes, copies inherited selection at shallow copy
	SelectAll( tBoolOp::opFalse );

	// Now add copies to m_Shapes
	for ( auto pShape : newShapes )	{ if ( pShape ) add2ShapeList( pShape ); }

	StaticModelChanged( );
}

void NNetModel::ConnectIncoming
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	if ( pPipe && pEndPoint )
	{
		pEndPoint->AddIncoming( pPipe );
		pPipe->SetEndKnot( pEndPoint );
	}
}

void NNetModel::ConnectOutgoing
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	if ( pPipe && pStartPoint )
	{
		pStartPoint->AddOutgoing( pPipe );
		pPipe->SetStartKnot( pStartPoint );
	}
}

ShapeId const NNetModel::FindShapeAt
( 
	MicroMeterPoint const & umPoint, 
	ShapeCrit       const & crit 
) const
{	
	ShapeId idRes { NO_SHAPE };

	if ( idRes == NO_SHAPE )   // first test all neurons and input neurons
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsAnyNeuron( ) && crit( s ); } );

	if ( idRes == NO_SHAPE )   // if nothing found, test knot shapes
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsKnot     ( ) && crit( s ); } ); 	

	if ( idRes == NO_SHAPE )   // if nothing found, try pipes
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsPipe     ( ) && crit( s ); } );

	return idRes;
}

/////////////////// local functions ///////////////////////////////////////////////

void NNetModel::selectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op )
{
	if ( pBaseKnot )
	{
		pBaseKnot->Select( op );
		pBaseKnot->Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) 
			{ 
				pipe.Select( op ); 
				BaseKnot * const pEndKnot { pipe.GetEndKnotPtr() };
				if ( pEndKnot->IsKnot() )
					selectSubtree( pipe.GetEndKnotPtr(), op ); 
			} 
		);
	}
}

void NNetModel::RemoveFromShapeList( Shape * const pShape )
{
	ShapeId id { pShape->GetId() };
	assert( m_Shapes[ id.GetValue() ] == pShape );
	m_Shapes[ id.GetValue() ] = nullptr;
}

void NNetModel::DisconnectBaseKnot( BaseKnot * const pBaseKnot ) // disconnects only, shape remains
{
	if ( pBaseKnot )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		pBaseKnot->Apply2AllInPipes
		( 
			[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->AddIncoming( & pipe );
				pipe.SetEndKnot( pKnotNew );
				pipe.DislocateEndPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->ClearIncoming();
		pBaseKnot->Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->AddOutgoing( & pipe );
				pipe.SetStartKnot( pKnotNew );
				pipe.DislocateStartPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->ClearOutgoing();
		assert( pBaseKnot->IsOrphan( ) );
	}
}

void NNetModel::deletePipeEndPoints( Pipe * const pPipe )
{
	BaseKnot * pStartKnot { pPipe->GetStartKnotPtr() };
	pStartKnot->RemoveOutgoing( pPipe );
	if ( pStartKnot->IsOrphanedKnot( ) && ( pStartKnot->IsKnot() ) )
	{
		RemoveFromShapeList( pStartKnot );
		delete pStartKnot;                       
	}

	BaseKnot * pEndKnot { pPipe->GetEndKnotPtr() };
	pEndKnot->RemoveIncoming( pPipe );
	if ( pEndKnot->IsOrphanedKnot( ) && ( pEndKnot->IsKnot() ) )
	{
		RemoveFromShapeList( pEndKnot );
		delete( pEndKnot );
	}
}

void NNetModel::InsertBaseKnot( Pipe * const pPipe, BaseKnot * const pBaseKnot)
{
	if ( pPipe && pBaseKnot )
	{
		BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr( ) };
		ShapeId    const idNewPipe  { NewPipe( pStartKnot, pBaseKnot ) };
		pStartKnot->RemoveOutgoing( pPipe );
		ConnectOutgoing( pPipe, pBaseKnot );
		pPipe->Recalc();
	}
}

MicroMeterPoint NNetModel::orthoVector( ShapeId const idPipe ) const
{
	return OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
}

ShapeId const NNetModel::findShapeAt
( 
	MicroMeterPoint const   pnt, 
	ShapeCrit       const & crit 
) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape->GetId();
	};
	return NO_SHAPE;
}

/////////////////// nonmember functions ///////////////////////////////////////////////

MicroMeterRect ComputeEnclosingRect( ShapeList const & shapeVector )
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
