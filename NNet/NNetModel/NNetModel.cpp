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

NNetModel::NNetModel( NNetModel const & modelSrc )
{
	* this = modelSrc;
	for ( auto pShapeSrc : modelSrc.m_Shapes )
	{
		if ( pShapeSrc )
		{
			m_Shapes.at( pShapeSrc->GetId().GetValue() ) = shallowCopy( * pShapeSrc );
		}
	}
	for ( auto pShapeSrc : modelSrc.m_Shapes )
	{
		if ( pShapeSrc )
		{
			connectToNewShapes( * pShapeSrc, m_Shapes );
		}
	}
	assert( IsEqual( modelSrc ) );
}

NNetModel::~NNetModel( )
{
	for ( auto pShape : m_Shapes )
		delete pShape;
	m_Shapes.clear();
}

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
}                     

void NNetModel::CreateInitialShapes( )
{
	InputNeuron * const pInputNeuron { NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	Pipe        * const pNewPipe     { NewPipe( pInputNeuron, pNeuron ) };
}

bool NNetModel::IsEqual( NNetModel const & other ) const
{
	size_t iMax { max( m_Shapes.size(), other.m_Shapes.size() ) };
	for ( int i = 0; i < iMax; ++i )
	{
		Shape const * pShape      { (i >=       m_Shapes.size()) ? nullptr :       m_Shapes[i] };
		Shape const * pShapeOther { (i >= other.m_Shapes.size()) ? nullptr : other.m_Shapes[i] };
		if ( (pShape == nullptr) != (pShapeOther == nullptr) )
		{
			return false;
		}
		else if ( (pShape != nullptr) && (pShapeOther != nullptr) )
		{
			if ( ! isEqual( * pShape, * pShapeOther ) )
				return false;
		}
	}
	return true;
}

void NNetModel::RecalcAllShapes( ) 
{ 
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Recalc( ); } );
	dynamicModelChanged( );
} 

void NNetModel::GetSelectionList( ShapeList & list ) const
{
	list.clear();
	Apply2All<Shape>
	( 
		[&]( Shape & shape ) 
		{ 
			if ( shape.IsSelected() )
				list.push_back( & shape );
		} 
	);
}

void NNetModel::SetSelectionList( ShapeList const & list )
{
	SelectAll( tBoolOp::opFalse );
	for ( Shape * pShape : list )
		pShape->Select( tBoolOp::opTrue );
}

void NNetModel::DeleteShape( Shape * const pShape )
{
	if ( pShape )
	{
		if ( pShape->IsPipe() )
		{
			Pipe     * const pPipe      { Cast2Pipe( pShape ) };
			BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr() };
			pStartKnot->m_connections.RemoveOutgoing( pPipe );
			if ( pStartKnot->IsOrphanedKnot( ) && ( pStartKnot->IsKnot() ) )
			{
				RemoveFromShapeList( pStartKnot );
				delete pStartKnot;                       
			}

			BaseKnot * pEndKnot { pPipe->GetEndKnotPtr() };
			pEndKnot->m_connections.RemoveIncoming( pPipe );
			if ( pEndKnot->IsOrphanedKnot( ) && ( pEndKnot->IsKnot() ) )
			{
				RemoveFromShapeList( pEndKnot );
				delete( pEndKnot );
			}
		}
		else 
			DisconnectBaseKnot( Cast2BaseKnot( pShape ) );
		RemoveFromShapeList( pShape );
		delete pShape;
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

void NNetModel::ToggleStopOnTrigger( ShapeId const id )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( id ) } )
	{
		pNeuron->StopOnTrigger( tBoolOp::opToggle );
		dynamicModelChanged( );
	}
}

fHertz const NNetModel::GetPulseRate( ShapeId const id ) const
{
	InputNeuron const * const pInputNeuron { GetShapeConstPtr<InputNeuron const *>( id ) };
	return ( pInputNeuron )
		   ? pInputNeuron->GetPulseFrequency( )
	       : fHertz::NULL_VAL();
}

float NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	float fOldValue { m_pParam->GetParameterValue( param ) };
	m_pParam->SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
	return fOldValue;
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
		insertBaseKnot( GetShapePtr<Pipe *>( idDst ), pSrc );
	}
	else  // connect baseknot to baseknot
	{
		BaseKnot * pDstBaseKnot { static_cast<BaseKnot *>( pDst ) };
		if ( pSrc && pDstBaseKnot )
		{
			pSrc->m_connections.Apply2AllInPipes ( [&]( Pipe & pipe ) { ConnectIncoming( & pipe, pDstBaseKnot ); } );
			pSrc->m_connections.Apply2AllOutPipes( [&]( Pipe & pipe ) { ConnectOutgoing( & pipe, pDstBaseKnot ); } );
			RemoveFromShapeList( pSrc );
			delete pSrc;
		}
	}
	StaticModelChanged( );
}

Pipe * const NNetModel::NewPipe( BaseKnot * const pStart, BaseKnot * const pEnd )
{
	Pipe * const pPipe = new Pipe( pStart, pEnd );
	pStart->m_connections.AddOutgoing( pPipe );
	pEnd  ->m_connections.AddIncoming( pPipe );
	Add2ShapeList( pPipe );
	StaticModelChanged( );
	return pPipe;
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

void NNetModel::DeleteSelection( )
{
	for ( int i = 0; i < m_Shapes.size(); ++i )  // Caution!
	{	                                         // Range based loop does not work here
		Shape * p = m_Shapes.at(i);              // DeleteShape changes the range 
		if ( p &&  p->IsSelected() )             // by creating new shapes!!
			DeleteShape( p ); 
	}
	StaticModelChanged( );
}

void NNetModel::MarkSelection( tBoolOp const op )
{
	Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( op ); } );
	dynamicModelChanged();
}

bool NNetModel::isEqual( Shape const & shapeA, Shape const & shapeB ) const
{
	if ( shapeA.GetShapeType().GetValue() != shapeB.GetShapeType().GetValue() )
		return false;

	switch ( shapeA.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if (! static_cast<InputNeuron const &>( shapeA ).IsEqual( static_cast<InputNeuron const &>( shapeB ) ) )
			return false;
		break;

	case ShapeType::Value::knot:
		if ( ! static_cast<Knot const &>( shapeA ).IsEqual( static_cast<Knot const &>( shapeB ) ) )
			return false;
		break;

	case ShapeType::Value::neuron:
		if ( ! static_cast<Neuron const &>( shapeA ).IsEqual( static_cast<Neuron const &>( shapeB ) ) )
			return false;
		break;

	case ShapeType::Value::pipe:
		if ( ! static_cast<Pipe const &>( shapeA ).IsEqual( static_cast<Pipe const &>( shapeB ) ) )
			return false;
		break;

	default:
		assert( false );
		return false;
	}
	return true;
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

void NNetModel::connectToNewShapes( Shape const & shapeSrc, ShapeList const & newShapes ) const 
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
		Connections const & srcConn { static_cast<BaseKnot const &>( shapeSrc ).m_connections };
		Connections       & dstConn { static_cast<BaseKnot       &>( shapeDst ).m_connections };
		dstConn.ClearOutgoing();
		dstConn.ClearIncoming();
		auto dstFromSrc = [&](Pipe const & pipeSrc){ return static_cast<Pipe *>(newShapes.at(pipeSrc.GetId().GetValue())); };
		srcConn.Apply2AllOutPipes( [&]( Pipe const & pipeSrc ) { dstConn.AddOutgoing( dstFromSrc( pipeSrc ) ); } );
		srcConn.Apply2AllInPipes ( [&]( Pipe const & pipeSrc ) { dstConn.AddIncoming( dstFromSrc( pipeSrc ) ); } );
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
	for ( auto pShape : newShapes )	{ if ( pShape ) Add2ShapeList( pShape ); }

	StaticModelChanged( );
}

void NNetModel::SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op )
{
	if ( pBaseKnot )
	{
		pBaseKnot->Select( op );
		pBaseKnot->m_connections.Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) 
			{ 
				pipe.Select( op ); 
				if ( pipe.GetEndKnotPtr()->IsKnot() )
					SelectSubtree( pipe.GetEndKnotPtr(), op ); 
			} 
		);
	}
}

void NNetModel::DisconnectBaseKnot( BaseKnot * const pBaseKnot ) // disconnects only, shape remains
{
	if ( pBaseKnot )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		pBaseKnot->m_connections.Apply2AllInPipes
		( 
			[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->m_connections.AddIncoming( & pipe );
				pipe.SetEndKnot( pKnotNew );
				pipe.DislocateEndPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->m_connections.ClearIncoming();
		pBaseKnot->m_connections.Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->m_connections.AddOutgoing( & pipe );
				pipe.SetStartKnot( pKnotNew );
				pipe.DislocateStartPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->m_connections.ClearOutgoing();
		assert( pBaseKnot->m_connections.IsOrphan( ) );
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

void NNetModel::insertBaseKnot( Pipe * const pPipe, BaseKnot * const pBaseKnot)
{
	BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr( ) };
	NewPipe( pStartKnot, pBaseKnot );
	pStartKnot->m_connections.RemoveOutgoing( pPipe );
	ConnectOutgoing( pPipe, pBaseKnot );
}

void NNetModel::setTriggerSound( Neuron * const pNeuron, bool const bActive, Hertz const freq, MilliSecs const msec )
{
	if ( pNeuron )
	{
		pNeuron->SetTriggerSoundOn       ( bActive );
		pNeuron->SetTriggerSoundFrequency( freq );
		pNeuron->SetTriggerSoundDuration ( msec );
		StaticModelChanged( );
	}
}

void NNetModel::clearTriggerSound( Neuron * const pNeuron )
{
	setTriggerSound( pNeuron, false, 0_Hertz, 0_MilliSecs );
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

void ConnectIncoming
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	pEndPoint->m_connections.AddIncoming( pPipe );
	pPipe->SetEndKnot( pEndPoint );
	pPipe->Recalc( );
}

void ConnectOutgoing
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	pStartPoint->m_connections.AddOutgoing( pPipe );
	pPipe->SetStartKnot( pStartPoint );
	pPipe->Recalc( );
}

void ReplaceStartKnot
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	BaseKnot * const pOldStartPoint { pPipe->GetStartKnotPtr() };
	pOldStartPoint->m_connections.RemoveIncoming( pPipe ); 
	pPipe->SetStartKnot( pStartPoint );
	pPipe->Recalc( );
}

void ReplaceEndKnot  
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	BaseKnot * const pOldEndPoint { pPipe->GetEndKnotPtr() };
	pOldEndPoint->m_connections.RemoveIncoming( pPipe ); 
	pPipe->SetEndKnot( pEndPoint );
	pPipe->Recalc( );
}
